File Formats
============

Brainiac uses two main text-based file formats for storing configuration data:

BAF (Brainiac Agent Format)
---------------------------

BAF files store individual agent definitions including brain (fuzzy logic networks) and body (skeleton) structures.

File Structure
~~~~~~~~~~~~~~

::

    <segment_blocks>
    <fuzzy_blocks>
    <connection_blocks>
    End

Segment Blocks (Body/Skeleton Definition)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Segment blocks define the skeletal structure of agents, including bones, their hierarchy, and geometric properties.

::

    segment <bone_name>
        parent <parent_bone_name>
        translation <x> <y> <z>
        rotation <x> <y> <z>
        primitive <BOX|other_primitives>
        size <x> <y> <z>          # For BOX primitives only
        editorpos <x> <y>         # Visual editor position
    endSegment

**Fields:**

- ``bone_name``: Unique identifier for the bone
- ``parent``: Name of parent bone (same as bone_name for root bones)
- ``translation``: 3D position offset from parent bone
- ``rotation``: 3D rotation in degrees
- ``primitive``: Geometric primitive type (currently only BOX supported)
- ``size``: Dimensions for BOX primitives (width, height, depth)
- ``editorpos``: Position in visual editor (x, y coordinates)

Fuzzy Blocks (Brain/Logic Definition)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Fuzzy blocks define the neural network nodes that control agent behavior.

::

    fuzz <fuzzy_node_name>
        type <AND|OR|INPUT|OUTPUT|NOISE>
        mode <MAX|SUM|MIN|PRODUCT>    # For AND/OR nodes
        rate <float_value>            # For NOISE nodes
        channel <channel_name>        # For OUTPUT nodes
        editorpos <x> <y>            # Visual editor position
    endFuzz

**Node Types:**

- ``AND``: Logical AND operation with modes MIN or PRODUCT
- ``OR``: Logical OR operation with modes MAX or SUM
- ``INPUT``: Input from agent sensors
- ``OUTPUT``: Output to agent actuators
- ``NOISE``: Random value generator with configurable rate

**Channels:**

Output nodes connect to specific channels that control agent behavior:

- ``RX``, ``RY``, ``RZ``: Rotation around X, Y, Z axes
- ``TX``, ``TY``, ``TZ``: Translation along X, Y, Z axes
- ``COLOR``: Visual appearance

Connection Blocks (Neural Network Connections)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Connection blocks define how fuzzy logic nodes are connected to form neural networks.

::

    connections <parent_fuzzy_name>
        child <child_fuzzy_name> <inverted_flag>
        child <child_fuzzy_name> <inverted_flag>
        ...
    endConnections

**Fields:**

- ``parent_fuzzy_name``: Name of the parent node
- ``child_fuzzy_name``: Name of the child node
- ``inverted_flag``: 0 for normal connection, 1 for inverted connection

BSF (Brainiac Scene Format)
---------------------------

BSF files store complete scene definitions including agents and generators that place agent instances in the world.

File Structure
~~~~~~~~~~~~~~

::

    <agent_blocks>
    <generator_blocks>
    End

Agent Blocks
~~~~~~~~~~~~

Agent blocks reference BAF files that define individual agent types.

::

    agent <agent_name>
        fileName <relative_path_to_baf_file>
    endAgent

**Fields:**

- ``agent_name``: Unique identifier for the agent type
- ``fileName``: Relative path to the corresponding BAF file

Generator Blocks
~~~~~~~~~~~~~~~~

Generator blocks define how agent instances are placed and distributed in the simulation world.

::

    generator
        type <GENERATORPOINT>
        agentName <agent_name> <ratio>
        centerPoint <x> <y> <z>      # For GENERATORPOINT type
        gap <float_value>
        numAgents <integer>
        distance <float_value>
        rows <integer>
        columns <integer>
        angle <float_value>
        angleVariation <float_value>
        height <float_value>
        heightVariation <float_value>
        noise <float_value>
    endGenerator

**Fields:**

- ``type``: Generator type (currently only GENERATORPOINT supported)
- ``agentName``: Reference to agent type and spawn ratio
- ``centerPoint``: Center position for agent placement grid
- ``gap``: Spacing between agent positions
- ``numAgents``: Total number of agents to spawn
- ``distance``: Base distance between agents
- ``rows``, ``columns``: Grid dimensions for agent placement
- ``angle``, ``angleVariation``: Base rotation and random variation
- ``height``, ``heightVariation``: Base height and random variation
- ``noise``: Random placement noise factor

Runtime Data (AgentInstance)
----------------------------

AgentInstance objects are runtime instances created from Agent templates and are not directly saved/loaded. They contain:

**Channels**
    Input/output channels for communication:
    
    - **Rotation channels**: RX, RY, RZ (rotation around axes)
    - **Translation channels**: TX, TY, TZ (movement along axes)  
    - **Visual channels**: COLOR (appearance)

**Position/Orientation**
    Current 3D position and rotation vectors updated each simulation frame

**Brain Instance**
    Runtime fuzzy logic processor that executes the neural network

**Body Instance** 
    Runtime skeleton/bone system for 3D visualization

**Unique ID**
    Thread-safe unique identifier for each agent instance

Format Characteristics
----------------------

**Text-Based Format**
    Human-readable space-separated values for easy editing and debugging

**Hierarchical Structure**
    Bones form parent-child relationships, fuzzy nodes connect in networks

**Indentation**
    4-space indentation used for nested properties within blocks

**Relative Paths**
    Scene files reference agent files using relative paths for portability

**Type Safety**
    Uses Qt's QMetaEnum system for reliable type serialization

**3D Coordinates**
    All positions use QVector3D format with (x, y, z) coordinates