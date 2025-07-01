# Simple Setup Example

This directory contains the "simpleSetup" example with BSF (Brainiac Scene Format) and BAF (Brainiac Agent Format)
files.

## File Structure

```
examples/simpleSetup/
├── simpleSetup.bsf            # Main scene file
├── agent_files/               # Agent definition directory
│   ├── Agent1_Body.baf       # Agent with brain and body
│   ├── Agent2_NoBody.baf     # Agent with body only
│   └── Agent3_NoBody.baf     # Agent with body only
└── README.md                  # This file
```

## Files Description

### simpleSetup.bsf

Main scene file containing:

- 3 agent references pointing to BAF files in `agent_files/` directory
- 2 generators (GENERATORPOINT type) placing 400 agents each
- Generator 1: positioned at (-1100, 0, 200) in a 20x20 grid with 90 distance
- Generator 2: positioned at (2500, 0, -200) in a 20x20 grid with 90 distance

### agent_files/Agent1_Body.baf

Complete agent with brain and body:

- **Body**: 3 bones (root, left, right) with BOX primitives
- **Brain**: 6 fuzzy nodes including:
    - 3 NOISE nodes (including color noise)
    - 1 AND node (MIN mode)
    - 1 OR node (MAX mode)
    - 1 OUTPUT node (color channel)
- **Connections**: Neural network connecting noise to logic gates and output

### agent_files/Agent2_NoBody.baf

Agent with body only (no brain):

- **Body**: 4 bones (root2, left2, right2, top) with varying sizes
- Root bone: 40x40x40 size
- No fuzzy logic components

### agent_files/Agent3_NoBody.baf

Agent with body only (no brain):

- **Body**: 4 bones (root3, left3, right3, top3)
- Root positioned at (0, -100, 0) - offset from other agents
- No fuzzy logic components

## Format Features Demonstrated

### BAF Format Features:

- Agent metadata (name and editor position)
- Hierarchical bone structure with parent-child relationships
- BOX primitive definitions with 3D sizes
- Fuzzy logic neural networks with multiple node types
- Neural network connections with parent-child relationships
- Editor positioning for visual layout

### BSF Format Features:

- Agent references with relative file paths
- Multiple generator definitions
- GENERATORPOINT generator type with grid placement
- 3D positioning and spacing parameters
- Agent assignment to generators with ratios

## Loading the Files

To load these files in Brainiac:

1. Load the scene file: `simpleSetup.bsf`
2. The scene will automatically load the referenced BAF files from `agent_files/`
3. Generators will create agent instances in the simulation

These files follow the specifications in `docs/source/manual/structure/file-formats.rst` and demonstrate all major
features of the Brainiac file formats.