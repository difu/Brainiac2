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
                gridInterval: 10
            }
        }

        Node {
            id: originNode
            PerspectiveCamera {
                id: cameraNode
                z: 600
                y: 200
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
            visible: false
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

            function addAgentInstance(agentInstanceQVariant) {
                var shapeComponent = Qt.createComponent("AgentInstance.qml");
                if( shapeComponent.status !== Component.Ready ) {
                    if(shapeComponent.status === Component.Error) {
                        console.debug("ERROR while spawning AgentInstance: "+ shapeComponent.errorString());
                    }
                }
                // console.debug("Creating QML from QV Parameter (Pointer): " + agentInstanceQVariant);
                let instance = shapeComponent.createObject(agentInstanceSpawner,
                    { "agentInstance": agentInstanceQVariant});
                instances.push(instance);
                count = instances.length
            }
            Component.onCompleted: {
                // for (var i = 0; i < 10; ++i)
                //     agentInstanceSpawner.addAgentInstance()
            }
        }

        OrbitCameraController {
            origin: originNode
            camera: cameraNode
        }
    }
    DebugView {
        source: v3d
    }
}
