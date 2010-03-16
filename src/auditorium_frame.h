#ifndef _AUDITORUIM_FRAME_H_
#define _AUDITORIUM_FRAME_H_

#include "entities_list_widget.h"

class AuditoriumFrame : public EntitiesListWidget
{
public:
	AuditoriumFrame(DB::DataBase &db);
	~AuditoriumFrame();
private:
	//EntitiesListWidget m_AudList;
};

#endif

