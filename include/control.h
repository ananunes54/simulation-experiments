#ifndef CONTROL_INCLUDED_H
#define CONTROL_INCLUDED_H

#include <matrix.h>

namespace simulation
{
	struct state
	{	
		vector<3> linAcceleration = {0, 0, 0};
		vector<3> angAcceleration = {0, 0, 0};
		vector<3> spacialVelocity = {0, 0, 0};
		vector<3> spacialPosition = {0, 0, 0};
		char visibleAxis1;
		char visibleAxis2;
	};

	void SetInitialValues(state& s);
}

#endif
