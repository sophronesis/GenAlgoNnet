# Learning neural networks with genetic algorithm in test environment
This project is a toy example of using genetic algorithm in reinforcement learning of neural networks in some environment
Requirements:
* Visual Studio 2013
* glut

# Quickstart
Type 'r' for random initial parameters
or
Type 'l [path/to/file]' for loading population file from disk
On each scene 3 agents will be chosen and they will compete for food
After each scene information about agent competition will be displayer in following format:
```
[number of scene in population]| <agent 1 id> <- energy gathered| <agent 2 id> <- energy gathered| <agent 3 id> <- energy gathered|
```
Also after each generation will be displayed scoreboard with seperation of agents that will be present in next generation and that who will be replaced with crossover of genes 
After all generations all agents will be saved in [data/output.popl]
# Implementation Details
Each agent have 2 neural networks inside
* One choose one object from scene
* Second make changes in speed and angle of agent

in [data/settings.cfg](https://github.com/hardkun/GenAlgoNnet/blob/master/Genalg/data/settings.cfg) you can adjust parameters like:
* Scene wight
* Scene height
* Object render segments (number of lines in circle)
  for example 3 will make all objects appear as triangles, 4 as squares and so on.
* Generations
* Tics per scene
*	Population size
* Small food amount
*	Big food amount
* Small food size
* Big food size
* Small food energy
* Big food energy
* 2 Neural networks topology details
