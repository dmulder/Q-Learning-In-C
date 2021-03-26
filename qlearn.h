/*
   Copyright (C) 2020 Soham Bhure <soham.bhure18@vit.edu>
   Copyright (C) 2021 David Mulder <dmulder@samba.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#define RAN_LIM 500000

int randrange(int start, int stop, int step);
void q_learn_explore(double **qMatrix, int state,
		     int state_size, int action_size,
		     int (*select_action)(int state, int action_size),
		     double (*reward)(int state, int action));
void q_learn_exploit(double **qMatrix, int state,
		     int state_size, int action_size,
		     double (*reward)(int state, int action));
int q_learn(double **qMatrix, double epsilon, int state,
	    int state_size, int action_size,
	    int (*select_action)(int state, int action_size),
	    double (*reward)(int state, int action));
