import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import BrainiacViewer

Node {
    id: agentInstanceNode

    property variant agentInstance
    property real hue: Math.random()
    Model {
        id: agentInstanceModel
        visible: true
        //scale: Qt.vector3d(100, 100, 100)

        geometry: AgentInstaceGeometry {
            id: instanceGeom
            objectName: "AgentInstanceGeometry"
            agentInstance: agentInstanceNode.agentInstance
            onAgentTranslationChanged: agentInstanceModel.setPosition(instanceGeom.agentTranslation)
            onAgentRotationChanged: agentInstanceModel.setEulerRotation(instanceGeom.agentRotation)
            Component.onCompleted: {
                //agentInstanceModel.setEulerRotation(Qt.vector3d(0, 0, 100))
            }
        }
        materials: [
            DefaultMaterial {
                lineWidth: 1
                cullMode: DefaultMaterial.NoCulling
            }
        ]
        AxisHelper {
            enableXYGrid: false
            enableXZGrid: false
            enableYZGrid: false
            visible: true
        }
    }

}
