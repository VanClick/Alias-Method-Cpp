#include <stack>
#include <stdlib.h>
#include "AliasMethod.h"
using namespace std;

AliasMethod::AliasMethod(const double paramWeights[], const int paramSize)
	:m_size(paramSize)
{
	/* Begin by doing basic structural checks on the inputs. */
	if (paramWeights == NULL || paramSize <= 0)
		return;

	/* Allocate space for the probability and alias tables. */
	m_probability = new double[paramSize];
	m_alias = new int[paramSize];

	double sum = .0;
	for (int i = 0; i < paramSize; i++)
		sum += paramWeights[i];

	/* Compute the average probability and cache it for later use. */
	const double average = sum / paramSize;

	/* Make a copy of the probabilities list, since we will be making changes to it. */
	double* weights = new double[paramSize];
	for (int i = 0; i < paramSize; i++)
	{
		weights[i] = paramWeights[i];
	}

	/* Create two stacks to act as worklists as we populate the tables. */
	stack<int> small;
	stack<int> large;

	/* Populate the stacks with the input probabilities. */
	for (int i = 0; i < paramSize; i++) 
	{
		/* If the probability is below the average probability, then we add
		 * it to the small list; otherwise we add it to the large list. */
		if (weights[i] >= average)
			large.push(i);
		else
			small.push(i);
	}

	/* As a note: in the mathematical specification of the algorithm, we
	* will always exhaust the small list before the big list.  However,
	* due to floating point inaccuracies, this is not necessarily true.
	* Consequently, this inner loop (which tries to pair small and large
	* elements) will have to check that both lists aren't empty.
	*/
	while (!small.empty() && !large.empty()) 
	{
		/* Get the index of the small and the large probabilities. */
		int less = small.top();
		small.pop();
		int more = large.top();

		/* These probabilities have not yet been scaled up to be such that
		 * 1/n is given weight 1.0.  We do this here instead.*/
		m_probability[less] = weights[less] / average;
		m_alias[less] = more;

		/* Decrease the probability of the larger one by the appropriate amount. */
		weights[more] = weights[more] + weights[less] - average;

		/* If the new probability is less than the average, add it into the small list */
		if (weights[more] < average)
		{
			large.pop();
			small.push(more);
		}
	}

	/* At this point, everything is in one list, which means that the
	 * remaining probabilities should all be 1/n.  Based on this, set them
	 * appropriately.  Due to numerical issues, we can't be sure which
	 * stack will hold the entries, so we empty both. */
	while (!small.empty())
	{
		m_probability[small.top()] = 1.0;
		m_alias[small.top()] = small.top();
		small.pop();
	}
	while (!large.empty())
	{
		m_probability[large.top()] = 1.0;
		m_alias[large.top()] = large.top();
		large.pop();
	}
}

AliasMethod::~AliasMethod()
{
	delete[] m_alias;
	delete[] m_probability;
}

int AliasMethod::next()
{
	/* Generate a fair die roll to determine which column to inspect. */
	int index = rand() % m_size;

	/* Generate a biased coin toss to determine which option to pick. */
	if (rand() <= m_probability[index] * RAND_MAX)
		return index;
	else
		return m_alias[index];
}
