import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import BrainiacViewer

Node {
    id: agentInstanceNode

    // property real xRotation: Math.random() * (360 - (-360)) + -360
    // property real yRotation: Math.random() * (360 - (-360)) + -360
    // property real zRotation: Math.random() * (360 - (-360)) + -360
    property variant agentInstance

    property real hue: Math.random()
    Model {
        id: agentInstanceModel
        visible: true
        //scale: Qt.vector3d(100, 100, 100)

        geometry: AgentInstaceGeometry {
            objectName: "AgentInstanceGeometry"
            agentInstance: agentInstanceNode.agentInstance
            // parent: setY(100)
        }
        materials: [
            DefaultMaterial {
                lineWidth: 1
                cullMode: DefaultMaterial.NoCulling
            }
        ]
    }
    AxisHelper {
        enableXYGrid: false
        enableXZGrid: false
        enableYZGrid: false
        visible: true
    }
}
