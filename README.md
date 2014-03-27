These are some simple utilities I've found useful for Cocos2d-x and Box2D.

GGUtil: Miscellaneous utility functions.

b2AStarFindPath: a general-purpose, positive-weight A* pathfinding
implementation for Box2D.

CCApplyForce: An action that applies force to a body.

CCEventAction: An action that posts an event.

CCSoundAction: An action that makes a sound effect.

CCWait: An action that does nothing. Useful because Cocos2d-x-3.0 made
CCActionInterval truly abstract, so you can't use it in a CCSequence to give
another action a delay.