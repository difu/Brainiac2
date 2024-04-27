import QtQuick
import QtQuick3D

import BrainiacViewer

Node {
    id: agentInstanceNode

    property real xRotation: Math.random() * (360 - (-360)) + -360
    property real yRotation: Math.random() * (360 - (-360)) + -360
    property real zRotation: Math.random() * (360 - (-360)) + -360

    property real hue: Math.random()
    Model {
        id: agentInstance
        visible: true
        scale: Qt.vector3d(100, 100, 100)
        geometry: AgentInstaceGeometry {
            objectName: "AgentInstaceGeometry"
        }
        materials: [
            DefaultMaterial {
                lineWidth: 1
            }
        ]
    }
}
