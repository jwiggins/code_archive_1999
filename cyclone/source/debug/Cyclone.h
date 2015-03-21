// ============================================================================= 
//   CyApplication.h                                           Created: 04/09/98 
// 
//   Application core that quits with no windows. 
// 
//   History: 
//      04/09/98  mnl    conception 
// =============================================================================

#include <Application.h>

#ifndef CYCLONE_H_

class CyApplication : public BApplication
{
public:
			CyApplication(const char *signature);
			~CyApplication();
			
	void		Pulse();
};

#define CYCLONE_H_ 1
#endif