import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import QtQuick3D.Helpers

import BrainiacViewer

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: "Viewer"

    property bool isLandscape: width > height

    View3D {
        id: v3d
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        camera: camera

        environment: SceneEnvironment {
            id: env
            backgroundMode: SceneEnvironment.Color
            clearColor: "#002b36"
            InfiniteGrid {
                gridInterval: 100
            }
        }

        Node {
            id: originNode
            PerspectiveCamera {
                id: cameraNode
                z: 600
            }
        }

        DirectionalLight {
            id: directionalLight
            color: Qt.rgba(0.4, 0.2, 0.6, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
        }

        PointLight {
            id: pointLight
            position: Qt.vector3d(0, 0, 100)
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
        }

        Model {
            id: gridModel
            visible: true
            scale: Qt.vector3d(100, 100, 100)
            geometry: GridGeometry {
                id: grid
                horizontalLines: 20
                verticalLines: 20
            }
            materials: [
                DefaultMaterial {
                    lineWidth: 1
                }
            ]
        }

        Model {
            id: triangleModel
            visible: true
            scale: Qt.vector3d(100, 100, 100)
            geometry: AgentInstaceGeometry {
            }
            materials: [
                DefaultMaterial {
                    lineWidth: 1
                }
            ]
        }

        Node {
            objectName: "agentInstanceSpawner"
            id: agentInstanceSpawner
            property real range: 300
            property var instances: []
            property int count

            function debugMsg(msg: string)
            {
                console.log("Received Message " + msg)
                return "Hello from agentSpawner"
            }

            function addAgentInstance()
            {
                var xPos = (2 * Math.random() * range) - range;
                var yPos = (2 * Math.random() * range) - range;
                var zPos = (2 * Math.random() * range) - range;
                var shapeComponent = Qt.createComponent("AgentInstance.qml");

                if( shapeComponent.status !== Component.Ready ) {
                    if(shapeComponent.status === Component.Error) {
                        console.debug("ERROR while spawning AgentInstance: "+ shapeComponent.errorString());
                    }
                }

                let instance = shapeComponent.createObject(agentInstanceSpawner,
                    { "x": xPos, "y": yPos, "z": zPos, "scale": Qt.vector3d(0.25, 0.25, 0.25)});
                instances.push(instance);
                count = instances.length
            }
            Component.onCompleted: {
                for (var i = 0; i < 10; ++i)
                    agentInstanceSpawner.addAgentInstance()
            }
        }

        OrbitCameraController {
            origin: originNode
            camera: cameraNode
        }
    }
}
