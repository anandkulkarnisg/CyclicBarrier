#!/bin/bash
for iter in $(seq 1000)
do
	./Example7.out
done|sort|uniq
