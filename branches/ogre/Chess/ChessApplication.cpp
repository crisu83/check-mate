#include "stdafx.h"
#include "ChessApplication.h"

//-------------------------------------------------------------------------------------
ChessApplication::ChessApplication(void)
{
}

//-------------------------------------------------------------------------------------
ChessApplication::~ChessApplication(void)
{
}

//-------------------------------------------------------------------------------------
void ChessApplication::createCamera(void)
{
	// create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

	// set its position, direction  
    mCamera->setPosition(Ogre::Vector3(16.8, 80, 60));
    mCamera->lookAt(Ogre::Vector3(16.8, 0, 16.8));

	// set the near clip distance
    mCamera->setNearClipDistance(5);

	// create a default camera controller
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
 
//-------------------------------------------------------------------------------------
void ChessApplication::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	double delta, scale, x, y, z;

	delta = 4.8;
	scale = 80;
	x = 16.8;
	y = 0;
	z = 16.8;

	Ogre::SceneNode *boardNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Boardnode");
	Ogre::Entity* boardEntity = mSceneMgr->createEntity("Board", "chessboard.mesh");
	boardEntity->setCastShadows(false);
	boardNode->attachObject(boardEntity);
	boardNode->setScale(scale, scale, scale);
	boardNode->setPosition(x, y, z);

	// DARK SIDE

	y = 1.5;
	z = 0;

	Ogre::SceneNode* blackRookNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackRookNode");
	Ogre::Entity* blackRookEntity = mSceneMgr->createEntity("BlackRook", "rook.mesh");
	blackRookEntity->setCastShadows(true);
	blackRookNode->attachObject(blackRookEntity);
	blackRookNode->setScale(scale, scale, scale);
	blackRookNode->setPosition(0, y, z);

	Ogre::SceneNode* blackKnightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackKnightNode");
	Ogre::Entity* blackKnightEntity = mSceneMgr->createEntity("BlackKnight", "knight.mesh");
	blackKnightNode->attachObject(blackKnightEntity);
	blackKnightNode->setScale(scale, scale, scale);
	blackKnightNode->setPosition(delta, y, z);

	Ogre::SceneNode* blackBishopNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackBishopNode");
	Ogre::Entity* blackBishopEntity = mSceneMgr->createEntity("BlackBishop", "bishop.mesh");
	blackBishopNode->attachObject(blackBishopEntity);
	blackBishopNode->setScale(scale, scale, scale);
	blackBishopNode->setPosition(delta * 2, y, z);

	Ogre::SceneNode* blackKingNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackKingNode");
	Ogre::Entity* blackKingEntity = mSceneMgr->createEntity("BlackKing", "king.mesh");
	blackKingNode->attachObject(blackKingEntity);
	blackKingNode->setScale(scale, scale, scale);
	blackKingNode->setPosition(delta * 3, y, z);

	Ogre::SceneNode* blackQueenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackQueenNode");
	Ogre::Entity* blackQueenEntity = mSceneMgr->createEntity("BlackQueen", "queen.mesh");
	blackQueenNode->attachObject(blackQueenEntity);
	blackQueenNode->setScale(scale, scale, scale);
	blackQueenNode->setPosition(delta * 4, y, z);

	Ogre::SceneNode* blackBishopNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackBishopNode2");
	Ogre::Entity* blackBishopEntity2 = mSceneMgr->createEntity("BlackBishop2", "bishop.mesh");
	blackBishopNode2->attachObject(blackBishopEntity2);
	blackBishopNode2->setScale(scale, scale, scale);
	blackBishopNode2->setPosition(delta * 5, y, z);

	Ogre::SceneNode* blackKnightNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackKnightNode2");
	Ogre::Entity* blackKnightEntity2 = mSceneMgr->createEntity("BlackKnight2", "knight_mirror.mesh");
	blackKnightNode2->attachObject(blackKnightEntity2);
	blackKnightNode2->setScale(scale, scale, scale);
	blackKnightNode2->setPosition(delta * 6, y, z);

	Ogre::SceneNode* blackRookNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackRookNode2");
	Ogre::Entity* blackRookEntity2 = mSceneMgr->createEntity("BlackRook2", "rook.mesh");
	blackRookNode2->attachObject(blackRookEntity2);
	blackRookNode2->setScale(scale, scale, scale);
	blackRookNode2->setPosition(delta * 7, y, z);

	z += delta;

	Ogre::SceneNode* blackPawnNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode");
	Ogre::Entity* blackPawnEntity = mSceneMgr->createEntity("BlackPawn", "pawn.mesh");
	blackPawnNode->attachObject(blackPawnEntity);
	blackPawnNode->setScale(scale, scale, scale);
	blackPawnNode->setPosition(0, y, z);

	Ogre::SceneNode* blackPawnNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode2");
	Ogre::Entity* blackPawnEntity2 = mSceneMgr->createEntity("BlackPawn2", "pawn.mesh");
	blackPawnNode2->attachObject(blackPawnEntity2);
	blackPawnNode2->setScale(scale, scale, scale);
	blackPawnNode2->setPosition(delta, y, z);

	Ogre::SceneNode* blackPawnNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode3");
	Ogre::Entity* blackPawnEntity3 = mSceneMgr->createEntity("BlackPawn3", "pawn.mesh");
	blackPawnNode3->attachObject(blackPawnEntity3);
	blackPawnNode3->setScale(scale, scale, scale);
	blackPawnNode3->setPosition(delta * 2, y, z);

	Ogre::SceneNode* blackPawnNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode4");
	Ogre::Entity* blackPawnEntity4 = mSceneMgr->createEntity("BlackPawn4", "pawn.mesh");
	blackPawnNode4->attachObject(blackPawnEntity4);
	blackPawnNode4->setScale(scale, scale, scale);
	blackPawnNode4->setPosition(delta * 3, y, z);

	Ogre::SceneNode* blackPawnNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode5");
	Ogre::Entity* blackPawnEntity5 = mSceneMgr->createEntity("BlackPawn5", "pawn.mesh");
	blackPawnNode5->attachObject(blackPawnEntity5);
	blackPawnNode5->setScale(scale, scale, scale);
	blackPawnNode5->setPosition(delta * 4, y, z);

	Ogre::SceneNode* blackPawnNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode6");
	Ogre::Entity* blackPawnEntity6 = mSceneMgr->createEntity("BlackPawn6", "pawn.mesh");
	blackPawnNode6->attachObject(blackPawnEntity6);
	blackPawnNode6->setScale(scale, scale, scale);
	blackPawnNode6->setPosition(delta * 5, y, z);

	Ogre::SceneNode* blackPawnNode7 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode7");
	Ogre::Entity* blackPawnEntity7 = mSceneMgr->createEntity("BlackPawn7", "pawn.mesh");
	blackPawnNode7->attachObject(blackPawnEntity7);
	blackPawnNode7->setScale(scale, scale, scale);
	blackPawnNode7->setPosition(delta * 6, y, z);

	Ogre::SceneNode* blackPawnNode8 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BlackPawnNode8");
	Ogre::Entity* blackPawnEntity8 = mSceneMgr->createEntity("BlackPawn8", "pawn.mesh");
	blackPawnNode8->attachObject(blackPawnEntity8);
	blackPawnNode8->setScale(scale, scale, scale);
	blackPawnNode8->setPosition(delta * 7, y, z);

	// LIGHT SIDE

	z = delta * 6;

	Ogre::SceneNode* whitePawnNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode");
	Ogre::Entity* whitePawnEntity = mSceneMgr->createEntity("WhitePawn", "pawn.mesh");
	whitePawnNode->attachObject(whitePawnEntity);
	whitePawnNode->setScale(scale, scale, scale);
	whitePawnNode->setPosition(0, y, z);

	Ogre::SceneNode* whitePawnNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode2");
	Ogre::Entity* whitePawnEntity2 = mSceneMgr->createEntity("WhitePawn2", "pawn.mesh");
	whitePawnNode2->attachObject(whitePawnEntity2);
	whitePawnNode2->setScale(scale, scale, scale);
	whitePawnNode2->setPosition(delta, y, z);

	Ogre::SceneNode* whitePawnNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode3");
	Ogre::Entity* whitePawnEntity3 = mSceneMgr->createEntity("WhitePawn3", "pawn.mesh");
	whitePawnNode3->attachObject(whitePawnEntity3);
	whitePawnNode3->setScale(scale, scale, scale);
	whitePawnNode3->setPosition(delta * 2, y, z);

	Ogre::SceneNode* whitePawnNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode4");
	Ogre::Entity* whitePawnEntity4 = mSceneMgr->createEntity("WhitePawn4", "pawn.mesh");
	whitePawnNode4->attachObject(whitePawnEntity4);
	whitePawnNode4->setScale(scale, scale, scale);
	whitePawnNode4->setPosition(delta * 3, y, z);

	Ogre::SceneNode* whitePawnNode5 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode5");
	Ogre::Entity* whitePawnEntity5 = mSceneMgr->createEntity("WhitePawn5", "pawn.mesh");
	whitePawnNode5->attachObject(whitePawnEntity5);
	whitePawnNode5->setScale(scale, scale, scale);
	whitePawnNode5->setPosition(delta * 4, y, z);

	Ogre::SceneNode* whitePawnNode6 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode6");
	Ogre::Entity* whitePawnEntity6 = mSceneMgr->createEntity("WhitePawn6", "pawn.mesh");
	whitePawnNode6->attachObject(whitePawnEntity6);
	whitePawnNode6->setScale(scale, scale, scale);
	whitePawnNode6->setPosition(delta * 5, y, z);

	Ogre::SceneNode* whitePawnNode7 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode7");
	Ogre::Entity* whitePawnEntity7 = mSceneMgr->createEntity("WhitePawn7", "pawn.mesh");
	whitePawnNode7->attachObject(whitePawnEntity7);
	whitePawnNode7->setScale(scale, scale, scale);
	whitePawnNode7->setPosition(delta * 6, y, z);

	Ogre::SceneNode* whitePawnNode8 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhitePawnNode8");
	Ogre::Entity* whitePawnEntity8 = mSceneMgr->createEntity("WhitePawn8", "pawn.mesh");
	whitePawnNode8->attachObject(whitePawnEntity8);
	whitePawnNode8->setScale(scale, scale, scale);
	whitePawnNode8->setPosition(delta * 7, y, z);

	z += delta;

	Ogre::SceneNode* whiteRookNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteRookNode");
	Ogre::Entity* whiteRookEntity = mSceneMgr->createEntity("WhiteRook", "rook.mesh");
	whiteRookNode->attachObject(whiteRookEntity);
	whiteRookNode->setScale(scale, scale, scale);
	whiteRookNode->setPosition(0, y, z);

	Ogre::SceneNode* whiteKnightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteKnightNode");
	Ogre::Entity* whiteKnightEntity = mSceneMgr->createEntity("WhiteKnight", "knight.mesh");
	whiteKnightNode->attachObject(whiteKnightEntity);
	whiteKnightNode->setScale(scale, scale, scale);
	whiteKnightNode->setPosition(delta, y, z);
	whiteKnightNode->yaw(Ogre::Degree(180));

	Ogre::SceneNode* whiteBishopNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteBishopNode");
	Ogre::Entity* whiteBishopEntity = mSceneMgr->createEntity("WhiteBishop", "bishop.mesh");
	whiteBishopNode->attachObject(whiteBishopEntity);
	whiteBishopNode->setScale(scale, scale, scale);
	whiteBishopNode->setPosition(delta * 2, y, z);
	whiteBishopNode->yaw(Ogre::Degree(180));

	Ogre::SceneNode* whiteQueenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteQueenNode");
	Ogre::Entity* whiteQueenEntity = mSceneMgr->createEntity("WhiteQueen", "queen.mesh");
	whiteQueenNode->attachObject(whiteQueenEntity);
	whiteQueenNode->setScale(scale, scale, scale);
	whiteQueenNode->setPosition(delta * 3, y, z);

	Ogre::SceneNode* whiteKingNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteKingNode");
	Ogre::Entity* whiteKingEntity = mSceneMgr->createEntity("WhiteKing", "king.mesh");
	whiteKingNode->attachObject(whiteKingEntity);
	whiteKingNode->setScale(scale, scale, scale);
	whiteKingNode->setPosition(delta * 4, y, z);

	Ogre::SceneNode* whiteBishopNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteBishopNode2");
	Ogre::Entity* whiteBishopEntity2 = mSceneMgr->createEntity("WhiteBishop2", "bishop.mesh");
	whiteBishopNode2->attachObject(whiteBishopEntity2);
	whiteBishopNode2->setScale(scale, scale, scale);
	whiteBishopNode2->setPosition(delta * 5, y, z);
	whiteBishopNode2->yaw(Ogre::Degree(180));

	Ogre::SceneNode* whiteKnightNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteKnightNode2");
	Ogre::Entity* whiteKnightEntity2 = mSceneMgr->createEntity("WhiteKnight2", "knight_mirror.mesh");
	whiteKnightNode2->attachObject(whiteKnightEntity2);
	whiteKnightNode2->setScale(scale, scale, scale);
	whiteKnightNode2->setPosition(delta * 6, y, z);
	whiteKnightNode2->yaw(Ogre::Degree(180));

	Ogre::SceneNode* whiteRookNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("WhiteRookNode2");
	Ogre::Entity* whiteRookEntity2 = mSceneMgr->createEntity("WhiteRook2", "rook.mesh");
	whiteRookNode2->attachObject(whiteRookEntity2);
	whiteRookNode2->setScale(scale, scale, scale);
	whiteRookNode2->setPosition(delta * 7, y, z);
	
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(16.8, 30, 16.8);
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        ChessApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

