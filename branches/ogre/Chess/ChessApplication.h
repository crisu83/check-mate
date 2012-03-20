#ifndef __ChessApplication_h_
#define __ChessApplication_h_

#include "stdafx.h"
#include "BaseApplication.h"

class ChessApplication : public BaseApplication
{
public:
	ChessApplication(void);
	virtual ~ChessApplication(void);

protected:
	virtual void createScene(void);
	virtual void createCamera(void);
};

#endif