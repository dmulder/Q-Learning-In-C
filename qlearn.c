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

void q_matrix_init(struct q_matrix *q, int state_size, int action_size)
{
	q->matrix = malloc(sizeof(double*)*state_size);
	for (int i = 0; i < state_size; i++) {
		q->matrix[i] = malloc(sizeof(double)*action_size);
		memset(q->matrix[i], 0, sizeof(double)*action_size);
	}
	q->state_size = state_size;
	q->action_size = action_size;
}

void q_matrix_destroy(struct q_matrix *q)
{
	for (int i = 0; i < q->state_size; i++) {
		free(q->matrix[i]);
	}
	free(q->matrix);
}

int randrange(int start, int stop, int step)
{
	int width = (stop - start)/step;
	return start + ((rand() % width)*step);
}

static int q_matrix_max_action(struct q_matrix *q, int state)
{
	int *max_index = malloc(sizeof(int)*q->action_size);
	double temp_max = -DBL_MAX;
	int index_of_max = 0;
	int j = 0;
	for (int i = 0; i < q->action_size; i++) {
		max_index[i] = 0;

		if (temp_max == q->matrix[state][i]) {
			max_index[j] = i;
			j++;
		} else if (temp_max < q->matrix[state][i]) {
			j = 0;
			temp_max = q->matrix[state][i];
			max_index[j] = i;
			j++;
		}
	}

	/*
	 * Select a random out of all maximum
	 */
	index_of_max = max_index[rand() % j];
	free(max_index);
	return index_of_max;
}

static void update(int current_state, int action, struct q_matrix *q,
		   double gamma, double lr,
		   double (*reward)(int state, int action))
{
	/*
	 * Main update
	 */
	int max_action_index = q_matrix_max_action(q, current_state);
	double max_action = q->matrix[current_state][max_action_index];
	q->matrix[current_state][action] = reward(current_state, action) +
		lr * (gamma * max_action);
}

/*
 * Explore by selecting a random action
 */
void q_learn_explore(struct q_matrix *q, int state,
		     double gamma, double lr,
		     double (*reward)(int state, int action))
{
	int action = randrange(0, q->action_size, 1);
	update(state, action, q, gamma, lr, reward);
}

/*
 * Exploit by selecting the action with max value in q->matrix
 */
void q_learn_exploit(struct q_matrix *q, int state,
		     double gamma, double lr,
		     double (*reward)(int state, int action))
{
	int action = q_matrix_max_action(q, state);
	update(state, action, q, gamma, lr, reward);
}

/*
 * Explore and exploit, with an explore frequency of epsilon
 * Return the next recommended action
 */
int q_learn(struct q_matrix *q, double epsilon, int state,
	    double gamma, double lr,
	    double (*reward)(int state, int action))
{
	double choice = (double)rand() / (double)RAND_MAX;
	if (choice < epsilon) {
		q_learn_explore(q, state, gamma, lr, reward);
	} else {
		q_learn_exploit(q, state, gamma, lr, reward);
	}
	return q_matrix_max_action(q, state);
}
