
#ifndef PLUGTYPES_H_
#define PLUGTYPES_H_

//public member variables. need to make public so that appcore can do type testing.
    enum plugType_t {DUMMY, GENERIC, TRANSFORM, VOLUME, COLLISIONHINT, FLOAT, VECTOR, BOOLTYPE, EXTERNAL_FLOAT, EXTERNAL_VECTOR, COLOUR, ALPHA, ARB_CHANNEL} ;//different plugtypes to test if connection allowed. EXTERNAL is for plugs that are not affected by the graph so can be used in a cyclic way (downstream nodes plugging into upstream nodes
	enum plugDirection_t {IN,OUT};//So we can test that an outgoing plug can only connect to an incoming and vice versa.


#endif
