q:
	gcc -o q QLearningInC.c qlearn.c -g

clean:
	rm -rf q || echo
