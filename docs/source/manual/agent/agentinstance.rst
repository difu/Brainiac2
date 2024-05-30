AgentInstance
==============

The `AgentInstance` represents an instance of an agent in the simulation.
It is a child of `Agent` and contains the state of the individual agent in the simulation.
The state of an instance is the information that is needed to simulate the instance in the environment.

Each AgentInstance has a reference to the Agent that it is an instance of.
The AgentInstance is responsible for updating its state based on the environment and the other instances in the simulation.

The input channels can be used to update the state of the AgentInstance by its Brain.

Examples of the state of an AgentInstance are:

- **Position**: The position of the agent in the simulation space.
- **Velocity**: The velocity of the agent.
- **Orientation**: The orientation of the agent.


.. doxygenfile:: agentinstance.h
   :project: Brainiac C++
