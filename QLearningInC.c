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
#include "qlearn.h"

int main()
{
	int i, j;
	int initial_state, final_state = 7;
	int current_state, size_av_actions, action;
	double final_max = 0.0, scores[100000], score = 0.0;
	double qMatrix[8][8], rMatrix[8][8];
	int available_acts[8];

	printf("Enter the initial state: ");
	scanf("%d", &initial_state);

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			rMatrix[i][j] = -1.0;
			qMatrix[i][j] = 0.0;

			if ((i == 0 && j == 1) || (i == 1 && j == 5)
			    || (i == 5 && j == 6) || (i == 5 && j == 4)
			    || (i == 1 && j == 2) || (i == 2 && j == 3)
			    || (i == 2 && j == 7) || (i == 4 && j == 7)
			    || (i == 1 && j == 4)) {
				rMatrix[i][j] = 0.0;
			}

			if ((j == 0 && i == 1) || (j == 1 && i == 5)
			    || (j == 5 && i == 6) || (j == 5 && i == 4)
			    || (j == 1 && i == 2) || (j == 2 && i == 3)
			    || (j == 2 && i == 7) || (j == 4 && i == 7)
			    || (j == 1 && i == 4)) {
				rMatrix[i][j] = 0.0;
			}

			if ((i == 2 && j == 7) || (i == 7 && j == 7)
			    || (i == 4 && j == 7)) {
				rMatrix[i][j] = 100.0;
			}
		}
	}

	printf("\nPoints Matrix : \n");
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			printf("%f\t", rMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");

	printf("%f", rMatrix[7][7]);

	// Training the Q Matrix
	for (i = 0; i < 500; i++) {

		current_state = returnRandom();
		size_av_actions = available_actions(current_state,
						    available_acts, rMatrix);
		action = sample_next_action(size_av_actions, available_acts);

		score = update(current_state, action, rMatrix, qMatrix);
		scores[i] = score;

		printf("\nScore : %f", score);
	}

	//Finding the Max
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (final_max < qMatrix[i][j]) {
				final_max = qMatrix[i][j];
			}
		}
	}

	printf("\n\nTrained Q Matrix: \n");
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			printf("%f\t", (qMatrix[i][j] / final_max * 100.0));
		}
		printf("\n");
	}

	int curr_state = initial_state;
	int visited[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int no_way = 0;
	int row_max = 0, max_ind = 0;

	printf("Path: \n");
	while (visited[final_state] != 1) {
		printf("%d-> ", curr_state);
		row_max = 0;
		max_ind = 0;

		for (int i = 0; i < 8; i++) {
			if (visited[i] == 0) {
				if (qMatrix[curr_state][i] > row_max) {
					max_ind = i;
					row_max = qMatrix[curr_state][i];
				}
			}
		}

		curr_state = max_ind;
		visited[max_ind] = 1;
		if (row_max == 0) {
			no_way = 1;
			break;
		}

		if (curr_state == final_state) {
			break;
		}
	}

	if (no_way == 1) {
		printf("%d | There's no way after this\n");
	} else {
		printf("%d is the shortest path\n", curr_state);
	}

}
