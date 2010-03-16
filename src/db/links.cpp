#include "links.h"

namespace DB
{
	const Link_N2N g_LessonsAuditoriums("LesAud", g_Lessons, g_Auditoriums);
	const Link_N2N g_TeachersLessons("TeachLes", g_Teachers, g_Lessons);
	const Link_N2N g_OrderLessons("OrderLessons", g_Lessons, g_Lessons);
	const Link_TeachPlan g_TeachPlan("GroupLessons", g_Groups, g_TeachersLessons);
}
