/*
This file is part of UFO2000 (http://ufo2000.sourceforge.net)

Copyright (C) 2000-2001  Alexander Ivanov aka Sanami
Copyright (C) 2002       ufo2000 development team

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef CELL_H
#define CELL_H

#include "global.h"
#include "position.h"
#include "place.h"
#include "soldier.h"


/**
 * Minimal part of map. It can contain a soldier, stack of items, 
 * and also describes terrain objects (floor, object, two walls)
 * 
 * @ingroup battlescape
 */
class Cell: public persist::BaseObject
{
    DECLARE_PERSISTENCE(Cell);
private:
    int m_smog_time;
    int m_fire_time;
    int m_light;
	int m_visi;
	int islight; //!< 0 if not a light, otherwise set to power.
    Position m_position;

    int MOUSE;
    char visi[3][3][3];
    Soldier *m_soldier; //!< Soldier standing here
    Soldier *m_soldier_aux; //!< If another soldier stands here.
    Place *m_place;     //!< Stack of items here
public:
    //! Indexes of MCD records for floor, walls and object
	unsigned int type[4]; 
	
    Soldier *get_soldier() { return m_soldier; }
    Place *get_place() { return m_place; }
    void set_soldier(Soldier *soldier);
    bool soldier_here() { return m_soldier != NULL; }
    Position get_position() { return m_position; }

    Cell(Position p);
    virtual ~Cell();

    virtual bool Write(persist::Engine &archive) const;
    virtual bool Read(persist::Engine &archive);

    friend class Map;
};

#endif
