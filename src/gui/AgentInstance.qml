import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import BrainiacViewer
import QtQuick.Timeline

Node {
    id: agentInstanceNode

    property variant agentInstance
    property real hue: Math.random()

    // {{ skeleton }}
    Model {
        id: agentInstanceModel
        visible: true
        //scale: Qt.vector3d(100, 100, 100)
        geometry: AgentInstaceGeometry {
            id: instanceGeom
            objectName: "AgentInstanceGeometry"
            agentInstance: agentInstanceNode.agentInstance
            onAgentTranslationChanged: agentInstanceNode.setPosition(
                                           instanceGeom.agentTranslation)
            onAgentRotationChanged: agentInstanceNode.setEulerRotation(
                                        instanceGeom.agentRotation)
            Component.onCompleted: {

                //agentInstanceModel.setEulerRotation(Qt.vector3d(0, 0, 100))
            }
        }

        // {{ materials }}

        // {{ skin }}
        AxisHelper {
            id: agentAxisHelper
            enableXYGrid: false
            enableXZGrid: false
            enableYZGrid: false
            visible: false
            Component.onCompleted: {
                agentAxisAction.onTriggered.connect(function () {
                    agentAxisHelper.visible = !agentAxisHelper.visible
                })
            }
        }
    }
}
