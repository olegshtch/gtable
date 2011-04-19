#ifndef _LINE_EDITABLE_H_
#define _LINE_EDITABLE_H_

class LineEditable
{
public:
	virtual void add_empty_line() = 0;
	virtual void remove_line() = 0;
	virtual void update_model() = 0;
};

#endif
