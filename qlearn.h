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
#include <string.h>
#include <float.h>

struct q_matrix {
	double **matrix;
	int state_size;
	int action_size;
	int prev_state;
};

int randrange(int start, int stop, int step);
void q_matrix_init(struct q_matrix *q, int state_size, int action_size, int initial_state);
void q_matrix_destroy(struct q_matrix *q);
void q_learn_explore(struct q_matrix *q, int state,
		     double gamma, double lr,
		     double (*reward)(int state, int action));
void q_learn_exploit(struct q_matrix *q, int state,
		     double gamma, double lr,
		     double (*reward)(int state, int action));
int q_learn(struct q_matrix *q, double epsilon, int state,
	    double gamma, double lr,
	    double (*reward)(int state, int action));
