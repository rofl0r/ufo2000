/*
This file is part of "UFO 2000" aka "X-COM: Gladiators"
                    http://ufo2000.sourceforge.net/
Copyright (C) 2000-2001  Alexander Ivanov aka Sanami
Copyright (C) 2002-2004  ufo2000 development team

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

#include "global.h"
#include "scenario.h"
#include "multiplay.h"
#include "platoon.h"
#include "soldier.h"
#include "map.h"
#include "cell.h"
#include "place.h"
#include "units.h"
#include "item.h"

IMPLEMENT_PERSISTENCE(Scenario, "Scenario");

int explosives[3][8] = {{GRENADE, SMOKE_GRENADE, PROXIMITY_GRENADE, CANNON_HE_AMMO, CANNON_I_AMMO, AUTO_CANNON_HE_AMMO, AUTO_CANNON_I_AMMO, STUN_MISSILE},
						{HIGH_EXPLOSIVE, SMALL_ROCKET, INCENDIARY_ROCKET, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe},
						{ALIEN_GRENADE, LARGE_ROCKET, BLASTER_BOMB, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe}};

Scenario::Scenario (int sc_type)
{
	//we need to clear briefing strings in case we won't use all of them
	for (int i = 0; i < SCENARIO_NUMBER; i++) {
	    for (int j = 0; j < 5; j++) {
	        briefing_left[i][j] = "";
	        briefing_right[i][j] = "";
		}
	}


	name[SC_DEATHMATCH] = "DEATHMATCH";
	
	briefing_left[SC_DEATHMATCH][0] = "Kill 'em all!";
	
	briefing_right[SC_DEATHMATCH][0] = "Kill 'em all!";


	name[SC_ESCAPE] = "ESCAPE";
	
	briefing_left[SC_ESCAPE][0] = "You must get your leader (soldier #1)";
	briefing_left[SC_ESCAPE][1] = "to the opposite edge of the map or   ";
	briefing_left[SC_ESCAPE][2] = "kill enemy leader before he gets to  ";
	briefing_left[SC_ESCAPE][3] = "your edge.                           ";
	briefing_left[SC_ESCAPE][4] = "Leader can't have two-handed weapons.";
	
    briefing_right[SC_ESCAPE][0] = "You must get your leader (soldier #1)";
	briefing_right[SC_ESCAPE][1] = "to the opposite edge of the map or   ";
	briefing_right[SC_ESCAPE][2] = "kill enemy leader before he gets to  ";
	briefing_right[SC_ESCAPE][3] = "your edge.                           ";
	briefing_right[SC_ESCAPE][4] = "Leader can't have two-handed weapons.";


	name[SC_SABOTAGE] = "SABOTAGE";

	briefing_left[SC_SABOTAGE][0] = "You must place high explosives at    ";
	briefing_left[SC_SABOTAGE][1] = "indicated places.                    ";
	
	briefing_right[SC_SABOTAGE][0] = "You must kill all enemy soldiers     ";
	briefing_right[SC_SABOTAGE][1] = "before they place explosives.        ";
	
	
	name[SC_CONTROL] = "CONTROL";

	briefing_left[SC_CONTROL][0] = "You must control the 10x10 area in   ";
	briefing_left[SC_CONTROL][1] = "the centre of the map for 3 turns.   ";
	briefing_left[SC_CONTROL][2] = "To control an area you must keep in  ";
	briefing_left[SC_CONTROL][3] = "it more soldiers than your opponent. ";
	
    briefing_right[SC_CONTROL][0] = "You must control the 10x10 area in   ";
	briefing_right[SC_CONTROL][1] = "the centre of the map for 3 turns.   ";
	briefing_right[SC_CONTROL][2] = "To control an area you must keep in  ";
	briefing_right[SC_CONTROL][3] = "it more soldiers than your opponent. ";
	
	
	name[SC_ASSASSIN] = "ASSASSINATION";

	briefing_left[SC_ASSASSIN][0] = "You must kill enemy leader (first    ";
	briefing_left[SC_ASSASSIN][1] = "soldier).                            ";
	briefing_left[SC_ASSASSIN][2] = "Player 1 can deploy near all edges of";
	briefing_left[SC_ASSASSIN][3] = "the map (max. 5 tiles from an edge). ";
	briefing_left[SC_ASSASSIN][4] = "Player 2 deploys in centre (20x20).  ";
	
	briefing_right[SC_ASSASSIN][0] = "You must save your leader (first     ";
	briefing_right[SC_ASSASSIN][1] = "soldier).                            ";
	briefing_right[SC_ASSASSIN][2] = "Player 1 can deploy near all edges of";
	briefing_right[SC_ASSASSIN][3] = "the map (max. 5 tiles from an edge). ";
	briefing_right[SC_ASSASSIN][4] = "Player 2 deploys in centre (20x20).  ";
	
	
	name[SC_HOLD] = "HOLD";

	briefing_left[SC_HOLD][0] = "You must save at least half of your  ";
	briefing_left[SC_HOLD][1] = "squad until the end of the match (5  ";
	briefing_left[SC_HOLD][2] = "turns if turns limit is not set).    ";
	briefing_left[SC_HOLD][3] = "Player 1 deploys in centre (20x20),  ";
	briefing_left[SC_HOLD][4] = "Player 2 near edges (max. 5 tiles).  ";
	
	briefing_right[SC_HOLD][0] = "You must kill more than half of enemy";
	briefing_right[SC_HOLD][1] = "squad until the end of the match (5  ";
	briefing_right[SC_HOLD][2] = "turns if turns limit is not set).    ";
	briefing_right[SC_HOLD][3] = "Player 1 deploys in centre (20x20),  ";
	briefing_right[SC_HOLD][4] = "Player 2 near edges (max. 5 tiles).  ";


	name[SC_BREAK] = "BREAKTHROUGH";
	                              
	briefing_left[SC_BREAK][0] = "You must bring at least half of your ";
	briefing_left[SC_BREAK][1] = "platoon to the opposite edge of the  ";
	briefing_left[SC_BREAK][2] = "map.                                 ";
	
	briefing_right[SC_BREAK][0] = "You must kill more than half of enemy";
	briefing_right[SC_BREAK][1] = "platoon before they get to your edge ";
	briefing_right[SC_BREAK][2] = "of the map.                          ";
		
	
	rules[0] = 3;	//all explosives allowed
	rules[1] = 10;	//10k points limit
	rules[2] = 0;	//no turn limit
	rules[3] = 0;   //all map isn't explored
	
	
	new_scenario(sc_type);
}

void Scenario::new_scenario (int sc_type)
{
	if (sc_type == -1)
	    sc_type = SCENARIO_NUMBER - 1;
	    
	if (sc_type == SCENARIO_NUMBER)
		sc_type = SC_DEATHMATCH;

	type = sc_type;

	//g_console->printf(name[type]);
	//if (HOST)
	//	g_console->printf(briefing_left[type]);
	//else
	//	g_console->printf(briefing_right[type]);
		
	new_coords();
}


void Scenario::new_coords ()
{
	x1 = x2 = y1 = y2 = 0; //in case they won't be used
	
	switch (type) {
	    case SC_DEATHMATCH:
	    break;

	    case SC_ESCAPE:
	    break;

	    case SC_SABOTAGE:
		x1 = (rand() % ((mapdata.x_size - 2) * 10)) + 20;
		x2 = (rand() % ((mapdata.x_size - 2) * 10)) + 20;
		y1 = rand() % (mapdata.y_size * 10);
		y2 = rand() % (mapdata.y_size * 10);

		return;

	    case SC_CONTROL:
	    x1 = (mapdata.x_size * 10 - 10) / 2;
		x2 = x1 + 10;
  		y1 = (mapdata.y_size * 10 - 10) / 2;
        y2 = y1 + 10;
        return;
        
        case SC_ASSASSIN:
        break;
        
        case SC_HOLD:
        break;
        
        case SC_BREAK:
        break;
	}
}

int Scenario::check_conditions ()
{
	int n = 0;
	
	switch (type) {
		case SC_DEATHMATCH:
		break;

		case SC_ESCAPE:
		n = conditions_escape();
		break;

		case SC_SABOTAGE:
        n = conditions_sabotage();
		break;

		case SC_CONTROL:
        n = conditions_control();
		break;
		
		case SC_ASSASSIN:
		n = conditions_assassin();
		break;
		
		case SC_HOLD:
		n = conditions_hold();
		break;
		
		case SC_BREAK:
		n = conditions_break();
		break;
	}	
	if (n != 0) return n;
	
	n = conditions_common();
	if (n != 0) return n;
	
	return 0;
}

int Scenario::conditions_common ()
{
	int win = 0, loss = 0;
	
	int num_l = 0, num_r = 0;
	
    if (platoon_remote->captain() == NULL)
		win = 2;
		
	if (platoon_local->captain() == NULL)
		loss = 1;

	if (rules[2] > 0) {
		if ((turn / 2) + 1 > rules[2]) {
	        num_l = platoon_local->num_of_men();
	        num_r = platoon_remote->num_of_men();
	        
	        if (num_l >= num_r)
	            win = 2;
			else
			    loss = 1;
		}
	}

	return win + loss;
}

int Scenario::conditions_escape ()
{
	int win = 0, loss = 0;

    int x;
	Soldier *vip;
	
	if (platoon_remote->findman(1000) == NULL && platoon_remote->findman(2000) == NULL) {
		win = 2;
	}
	
	if (platoon_local->findman(1000) == NULL && platoon_local->findman(2000) == NULL) {
		loss = 1;
	}
	
  	if (p1->findman(1000) != NULL) {
		vip = p1->findman(1000);
		x = vip->x;
		if (x == (mapdata.x_size * 10) - 1) {
	    	if (platoon_local->belong(vip))
	        	win = 2;
			else
		    	loss = 1;
		}
	}
	
	if (p2->findman(2000) != NULL) {
		vip = p2->findman(2000);
		x = vip->x;
		if (x == 0) {
	    	if (platoon_local->belong(vip))
	        	win = 2;
			else
		    	loss = 1;
		}
	}
	
	return win + loss;
}

int Scenario::conditions_sabotage ()
{
	int win = 0, loss = 0;
	
    bool temp1b = false, temp2b = false;
	char buf[10000]; memset(buf, 0, sizeof(buf));
	int len = 0;
	
	//i don't know why x2/y2 must go before x1/y1, but it doesn't work otherwise
	map->cell(0, x2, y2)->get_place()->build_items_stats(buf, len);
	for (int i = 0; i < len; i++) {
	    if (buf[i] == HIGH_EXPLOSIVE) {
	    	temp2b = true;
	    	break;
		}
	}
	
	map->cell(0, x1, y1)->get_place()->build_items_stats(buf, len);
	for (int i = 0; i < len; i++) {
	    if (buf[i] == HIGH_EXPLOSIVE) {
	    	temp1b = true;
	    	break;
		}
	}

	if (temp1b && temp2b) {
	    if (p1 == platoon_local)
	        win = 2;
		else
		    loss = 1;
	}
	
 	return win + loss;
}

int Scenario::conditions_control ()
{
	int win = 0, loss = 0;
	
    int temp1 = 0, temp2 = 0;

    for (int i = 0; i < p1->num_of_men(); i++) {
  	    if (p1->findnum(i)->x >= x1 && p1->findnum(i)->x < x2 && p1->findnum(i)->y >= y1 && p1->findnum(i)->y < y2)
  	        temp1++;
	}
	for (int i = 0; i < p2->num_of_men(); i++) {
       	if (p2->findnum(i)->x >= x1 && p2->findnum(i)->x < x2 && p2->findnum(i)->y >= y1 && p2->findnum(i)->y < y2)
  	        temp2++;
	}
	if (temp1 > temp2) {
	    //in hotseat mode this check is performed twice more times than in internet mode
		if (net->gametype == GAME_TYPE_HOTSEAT)
  	    	turn_hold[0]++;
		else
		    turn_hold[0] += 2;
  	    turn_hold[1] = -1;
  	    g_console->printf("Player 1 is controlling the target area for %d turn(s).", (turn_hold[0] / 2) + 1);
  	}
	if (temp1 < temp2) {
  	    turn_hold[0] = -1;
  	    if (net->gametype == GAME_TYPE_HOTSEAT)
  	    	turn_hold[1]++;
		else
		    turn_hold[1] += 2;
  	    g_console->printf("Player 2 is controlling the target area for %d turn(s).", (turn_hold[1] / 2) + 1);
  	}
	if (temp1 == temp2) {
	    turn_hold[0] = -1;
	    turn_hold[1] = -1;
		g_console->printf("Noone is controlling the target area.");
    }
	if (turn_hold[0] == 4 && p1 == platoon_local)
	    win = 2;
	if (turn_hold[0] == 4 && p1 == platoon_remote)
	    loss = 1;
	if (turn_hold[1] == 4 && p2 == platoon_local)
	    win = 2;
	if (turn_hold[1] == 4 && p2 == platoon_remote)
	    loss = 1;
	    
    return win + loss;
}

int Scenario::conditions_assassin ()
{
	int win = 0, loss = 0;
	
    if (p2->findman(1000) == NULL && p2->findman(2000) == NULL) {
		if (p2 == platoon_local)
		    loss = 1;
		else
		    win = 2;
	}
	
	return win + loss;
}

int Scenario::conditions_hold ()
{
	int win = 0, loss = 0;
	
	int last_turn;
	rules[2] == 0 ? last_turn = 5 : last_turn = rules[2];
	
	if (p1->num_of_men() < (num_of_men / 2) + (num_of_men % 2)) {
	    if (p1 == platoon_local)
	        loss = 1;
		else
		    win = 2;
	} else {
		if ((turn / 2) + 1 > last_turn) {
			if (p1 == platoon_local)
				win = 2;
			else
				loss = 1;
		}
	}
	
	return win + loss;
}

int Scenario::conditions_break ()
{
	int win = 0, loss = 0;
	
	int escaped = 0;
	Soldier *sld;
	
	if (p1->num_of_men() < (num_of_men / 2) + (num_of_men % 2)) {
		if (p1 == platoon_local)
			loss = 1;
		else
			win = 2;
	} else {	
		for (int i = 0; i < mapdata.y_size * 10; i++) {
			sld = map->cell(0, (mapdata.x_size * 10) - 1, i)->get_soldier();
			if (sld != NULL) {
				if (p1->belong(sld))
					escaped++;
			}
		}
		if (escaped >= (num_of_men / 2) + (num_of_men % 2)) {
			if (p1 == platoon_local)
				win = 2;
			else
				loss = 1;
		}
	}
	
	return win + loss;
}
			

bool Scenario::is_target_on_minimap (int lev, int col, int row, Map *m_map)
{
	switch (type) {
    	case SC_DEATHMATCH:
	    break;
	    
    	case SC_ESCAPE:
		return minimap_escape(lev, col, row, m_map);
		break;
	
		case SC_SABOTAGE:
        return minimap_sabotage(col, row);
		break;
		
		case SC_CONTROL:
		break;
	
		case SC_ASSASSIN:
		return minimap_assassin(lev, col, row, m_map);
		break;
		
		case SC_HOLD:
		break;
		
		case SC_BREAK:
		break;
	}
	
	return false;
}

void Scenario::draw_minimap_rectangle (BITMAP *bmp, int x, int y)
{
	switch (type) {
		case SC_DEATHMATCH:
		break;
		
		case SC_ESCAPE:
		break;
		
		case SC_SABOTAGE:
		break;
		
		case SC_CONTROL:
		minimap_control(bmp, x, y);
		break;
		
		case SC_ASSASSIN:
		break;
		
		case SC_HOLD:
		break;
		
		case SC_BREAK:
		break;
	}
}

bool Scenario::minimap_escape (int lev, int col, int row, Map *m_map)
{
    if (m_map->man(lev, col, row) != NULL) {
		if (m_map->man(lev, col, row)->get_NID() == 1000 && platoon_remote->belong(m_map->man(lev, col, row)))
			return true;
		if (m_map->man(lev, col, row)->get_NID() == 2000 && platoon_remote->belong(m_map->man(lev, col, row)))
		    return true;
	}
	
	return false;
}

bool Scenario::minimap_sabotage (int col, int row)
{
    if (col == x1 && row == x1)
  		return true;
	if (col == x2 && row == y2)
	    return true;
	    
	return false;
}

void Scenario::minimap_control (BITMAP *bmp, int x, int y)
{
	rect(bmp, x + x1 * 4, y + y1 * 4, x + x2 * 4, y + y2 * 4, xcom1_color(1));
}

bool Scenario::minimap_assassin (int lev, int col, int row, Map *m_map)
{
    if (m_map->man(lev, col, row) != NULL) {
		if (m_map->man(lev, col, row)->get_NID() == 1000 && p2->belong(m_map->man(lev, col, row)) && p2 == platoon_remote)
			return true;
		if (m_map->man(lev, col, row)->get_NID() == 2000 && p2->belong(m_map->man(lev, col, row)) && p2 == platoon_remote)
		    return true;
	}

	return false;
}

bool Scenario::is_correct_platoon (long points, Platoon *platoon, char *first_soldier, PanPos pos, char buf[10000], int len, int num_of_men_sel)
{
	bool n = true;
	
	switch (type) {
	    case SC_DEATHMATCH:
	    break;
	    
	    case SC_ESCAPE:
	    n = platoon_escape(platoon, first_soldier);
	    break;
	    
	    case SC_SABOTAGE:
     	n = platoon_sabotage(pos, buf, len);
	    break;
	    
	    case SC_CONTROL:
	    break;
	    
	    case SC_ASSASSIN:
	    n = platoon_assassin(platoon, pos, first_soldier);
	    break;
	    
	    case SC_HOLD:
	    n = platoon_hold (pos, num_of_men_sel);
	    break;
	    
	    case SC_BREAK:
	    n = platoon_break (pos, num_of_men_sel);
	    break;
	}
	
	if (n == false) return n;
	
	n = platoon_common(points, buf, len);
	if (n == false) return n;
	
	return true;
}

bool Scenario::platoon_common (long points, char buf[10000], int len)
{
	if (points > rules[1] * 1000) {
	    g_console->printf("%d points limit exceeded!", rules[1] * 1000);
	    return false;
	}
	if(rules[0] < 3) {
		for (int i = 0; i < len; i++) {
			for (int j = rules[0]; j < 3; j++) {
			    for (int k = 0; k < 8; k++) {
			        if (buf[i] == explosives[j][k]) {
			            if (type != SC_SABOTAGE || explosives[j][k] != HIGH_EXPLOSIVE) {
							g_console->printf("%s isn't allowed.", Item::obdata_name(explosives[j][k]));
			            	return false;
						}
					}
				}
			}
		}
	}

	return true;
}

bool Scenario::platoon_escape (Platoon *platoon, char *first_soldier)
{
    char buf[10000]; memset(buf, 0, sizeof(buf));
	int len = 0;
	extern int weapon[];

    platoon->findman(first_soldier)->build_items_stats (buf, len);

	for (int w = 0; w < 40; w++) {
		int num = 0;
		for (int i = 0; i < len; i++) {
			if (weapon[w] == buf[i])
				num++;
		}
		if ((Item::obdata_twoHanded(weapon[w]) > 0) && (num > 0)) {
			g_console->printf("Soldier #1 can't have two-handed weapons!");
			return false;
		}
	}
	
	return true;
}

bool Scenario::platoon_sabotage (PanPos pos, char buf[10000], int len)
{
	int he_num = 0;
	
	for (int i = 0; i < len; i++) {
		if (buf[i] == HIGH_EXPLOSIVE)
			he_num++;
	}

	if (he_num < 2 && pos == POS_LEFT) {
	    g_console->printf("You must have at least 2 high explosives.");
		return false;
	} else
		return true;
}

bool Scenario::platoon_assassin (Platoon *platoon, PanPos pos, char *first_soldier)
{
    char buf[10000]; memset(buf, 0, sizeof(buf));
	int len = 0;
	extern int weapon[];

	if (pos == POS_RIGHT) {
		platoon->findman(first_soldier)->build_items_stats (buf, len);

		for (int w = 0; w < 40; w++) {
			int num = 0;
			for (int i = 0; i < len; i++) {
				if (weapon[w] == buf[i])
					num++;
			}
			if ((Item::obdata_twoHanded(weapon[w]) > 0) && (num > 0)) {
				g_console->printf("Soldier #1 can't have two-handed weapons!");
				return false;
			}
		}
	}

	return true;
}

bool Scenario::platoon_hold (PanPos pos, int num_of_men_sel)
{
	if (pos == POS_LEFT)
	    num_of_men = num_of_men_sel;

	return true;
}

bool Scenario::platoon_break (PanPos pos, int num_of_men_sel)
{
	if (pos == POS_LEFT)
	    num_of_men = num_of_men_sel;

	return true;
}

bool Scenario::is_correct_place (PanPos pos, int x, int y)
{
	switch (type) {
	    case SC_DEATHMATCH:
	    break;

	    case SC_ESCAPE:
	    break;

	    case SC_SABOTAGE:
	    break;

	    case SC_CONTROL:
	    break;

	    case SC_ASSASSIN:
	    return place_assassin(pos, x, y);
	    break;
	    
	    case SC_HOLD:
		return place_hold(pos, x, y);
		break;
		
		case SC_BREAK:
		break;
	}

	return place_common(pos, x, y);
}

bool Scenario::place_common (PanPos pos, int x, int y)
{
    if (pos == POS_LEFT) {
		if (x > 9)
			return false;
	} else {
		if (x < (mapdata.x_size - 1) * 10)
			return false;
	}
	
	return true;
}

bool Scenario::place_assassin (PanPos pos, int x, int y)
{
	if (pos == POS_LEFT) {
	    if (x < 5 || x > mapdata.x_size * 10 - 6 || y < 5 || y > mapdata.y_size * 10 - 6)
	        return true;
	} else {
	    if (x > (mapdata.x_size - 2) * 10 / 2 && x < (mapdata.x_size - 2) * 10 / 2 + 20 && y > (mapdata.y_size - 2) * 10 / 2 && y < (mapdata.y_size - 2) * 10 / 2 + 20)
	        return true;
	}
	
	return false;
}

bool Scenario::place_hold (PanPos pos, int x, int y)
{
	if (pos == POS_LEFT) {
        if (x > (mapdata.x_size - 2) * 10 / 2 && x < (mapdata.x_size - 2) * 10 / 2 + 20 && y > (mapdata.y_size - 2) * 10 / 2 && y < (mapdata.y_size - 2) * 10 / 2 + 20)
	        return true;
	} else {
	    if (x < 5 || x > mapdata.x_size * 10 - 6 || y < 5 || y > mapdata.y_size * 10 - 6)
	        return true;
	}

	return false;
}