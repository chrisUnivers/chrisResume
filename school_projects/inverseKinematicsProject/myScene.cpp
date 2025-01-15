////////////////////////////////////////////////////
// // Template code for  CSC 473
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <shared/defs.h>

#include "shared/opengl.h"

#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"

#include "ArmSimulator.h"
#include "Link.h"
#include "Particle.h"

//#include <util/jama/tnt_stopwatch.h>
//#include <util/jama/jama_lu.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char*)&g_testVariable,
	"",0,(char*)NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo(button, state);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button press event\n");

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		animTcl::OutputMessage(
			"My mouse received a mouse button release event\n");
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo =
		GlobalResourceManager::use()->getMouseButtonInfo();

	if (updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON)
	{
		animTcl::OutputMessage(
			"My mouse motion callback received a mousemotion event\n");
	}

}	// myMotion


void MakeScene(void)
{
	bool success;

	// We use a stripped down particle class to render joints
	Particle* fixed_hinge = new Particle("fixedHinge");
	fixed_hinge->setPosition(vec3(1.0, 2.0, 2.5));
	success = GlobalResourceManager::use()->addSystem(fixed_hinge, true);
	assert(success);

	// We use a stripped down particle class to render joints
	Particle* hinge = new Particle("hinge");
	hinge->setPosition(vec3(1.0, 2.0, 2.5));
	hinge->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(hinge, true);
	assert(success);

	// We use a stripped down particle class to render joints
	// And the end effector "hand"
	Particle* hand = new Particle("hand");
	hand->setPosition(vec3(-2.15, 2.0, 2.5));
	hand->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(hand, true);
	assert(success);

	// We use a stripped down particle class to render joints
	Particle* wrist = new Particle("wrist");
	/*wrist->setPosition(vec3(8, 0, 5.5;*/
	wrist->setPosition(vec3(-4.20, 2.0, 2.5));
	wrist->setColor(vec3(1.0, 1.0, 1.0));
	success = GlobalResourceManager::use()->addSystem(wrist, true);
	assert(success);

	Particle* head = new Particle("head");
	head->setPosition(vec3(3.0, -2.0, 5.5));
	head->setId(4);
	head->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(head, true);
	assert(success);

	Particle* body = new Particle("body");
	body->setPosition(vec3(3.0, -2.0, 5.5));
	body->setId(5);
	body->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(body, true);
	assert(success);

	Particle* rightArmOne = new Particle("rightArmOne");
	rightArmOne->setPosition(vec3(3.0, -2.0, 5.5));
	rightArmOne->setId(6);
	rightArmOne->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightArmOne, true);
	assert(success);

	Particle* rightArmTwo = new Particle("rightArmTwo");
	rightArmTwo->setPosition(vec3(3.0, -2.0, 5.5));
	rightArmTwo->setId(7);
	rightArmTwo->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightArmTwo, true);
	assert(success);

	Particle* rightArmThree = new Particle("rightArmThree");
	rightArmThree->setPosition(vec3(3.0, -2.0, 5.5));
	rightArmThree->setId(8);
	rightArmThree->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightArmThree, true);
	assert(success);

	Particle* leftLegOne = new Particle("leftLegOne");
	leftLegOne->setPosition(vec3(3.0, -2.0, 5.5));
	leftLegOne->setId(9);
	leftLegOne->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(leftLegOne, true);
	assert(success);

	Particle* leftLegTwo = new Particle("leftLegTwo");
	leftLegTwo->setPosition(vec3(3.0, -2.0, 5.5));
	leftLegTwo->setId(10);
	leftLegTwo->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(leftLegTwo, true);
	assert(success);

	Particle* leftLegThree = new Particle("leftLegThree");
	leftLegThree->setPosition(vec3(3.0, -2.0, 5.5));
	leftLegThree->setId(11);
	leftLegThree->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(leftLegThree, true);
	assert(success);

	Particle* rightLegOne = new Particle("rightLegOne");
	rightLegOne->setPosition(vec3(3.0, -2.0, 5.5));
	rightLegOne->setId(12);
	rightLegOne->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightLegOne, true);
	assert(success);

	Particle* rightLegTwo = new Particle("rightLegTwo");
	rightLegTwo->setPosition(vec3(3.0, -2.0, 5.5));
	rightLegTwo->setId(13);
	rightLegTwo->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightLegTwo, true);
	assert(success);

	Particle* rightLegThree = new Particle("rightLegThree");
	rightLegThree->setPosition(vec3(3.0, -2.0, 5.5));
	rightLegThree->setId(14);
	rightLegThree->setColor(vec3(0.0, 0.0, 0.0));
	success = GlobalResourceManager::use()->addSystem(rightLegThree, true);
	assert(success);

	// Links have end point positions, defined by the particles above at the joints and end effector
	// Really they just store the lengthof the link though
	Link* link1 = new Link("link1", ParticleRef(fixed_hinge), ParticleRef(hinge));
	success = GlobalResourceManager::use()->addSystem(link1, true);
	assert(success);

	Link* link2 = new Link("link2", ParticleRef(hinge), ParticleRef(hand));
	success = GlobalResourceManager::use()->addSystem(link2, true);
	assert(success);

	Link* link3 = new Link("link3", ParticleRef(hand), ParticleRef(wrist));
	success = GlobalResourceManager::use()->addSystem(link3, true);
	assert(success);


	// The arm simulator is where the action is. the "Arm" is a two link planar IK chain
	ArmSimulator* arm_simulator = new ArmSimulator("iksim", LinkRef(link1), LinkRef(link2), LinkRef(link3));
	arm_simulator->setStart(vec3(-5, 3, 0));
	arm_simulator->setEnd(vec3(0, 3, 0));
	success = GlobalResourceManager::use()->addSimulator(arm_simulator, true);
	assert(success);
}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{

	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	animTcl::OutputMessage("My key callback received a key press event\n");
	return;

}	// myKey

static int testGlobalCommand(ClientData clientData, Tcl_Interp* interp, int argc, myCONST_SPEC char** argv)
{
	animTcl::OutputMessage("This is a test command!");
	animTcl::OutputResult("100");
	return TCL_OK;

}	// testGlobalCommand

void mySetScriptCommands(Tcl_Interp* interp)
{

	// here you can register additional generic (they do not belong to any object) 
	// commands with the shell

	Tcl_CreateCommand(interp, "test", testGlobalCommand, (ClientData)NULL,
		(Tcl_CmdDeleteProc*)NULL);

}	// mySetScriptCommands
