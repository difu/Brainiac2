Brainiac File Structure
=======================

A Brainiac configuration consists of at least one file. Scene data is the basic file (.bsf). Global render settings,
camera and light info, setting info, and placement info can be found in this file. There are direct links to agent (.baf)
and terrain (.obj) files within the code of the scene file.

The agent file (.baf) contains information about the agent, such as skeleton and brain data, as well as skinning ("weights") data.
It includes links to action files (.bad), agent geometry (.obj), shaders and texture maps.

.. list-table:: Suggested directory structure
   :widths: 25 25 50
   :header-rows: 1

   * -
     - Top level
     - 2nd level
   * - **Directory**
     - name_of_your_project
     - agent_files
   * - **Contents**
     - Scene file (.bsf)
     - Brainiac Agent Files (.baf)
   * -
     -
     -
   * - **Directory**
     -
     - geometry
   * - **Contents**
     -
     - Agent Geometry Files (.obj)
   * -
     -
     -
   * - **Directory**
     -
     - actions
   * - **Contents**
     -
     - Agent action data (.bad)


.. toctree::
    :maxdepth: 2
    :glob:

    *
    brain/index
    body/index
