/*
 * lockoutTimer.c
 *
 *  Created on: Feb 21, 2015
 *      Author: JAMES
 */
#include "lockoutTimer.h"
#include "supportFiles/interrupts.h"
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"

#define LOCKOUT_DURATION 50000

static volatile bool runLockout;

// Standard init function.
void lockoutTimer_init() {
	runLockout = false;
}

// Calling this starts the timer.
void lockoutTimer_start() {
	runLockout = true;
}

void lockoutTimer_stop() {
	runLockout = false;
}

// Returns true if the timer is running.
bool lockoutTimer_running() {
	return runLockout;
}


enum LockoutTimerStates {
	waiting_st,
	lockout_st,
};

// Initialize trigger state
LockoutTimerStates LockoutTimerState = waiting_st;


void lockoutTimer_tick() {
	static uint32_t lockoutCounter;

	// State actions
	switch(LockoutTimerState) {
		case waiting_st:
			lockoutCounter=0;
			break;
		case lockout_st:
			lockoutCounter++;
			break;
		default:
			break;
	}

	switch(LockoutTimerState) { // Transitions

		case waiting_st: // init state
			if (lockoutTimer_running()) {
				LockoutTimerState = lockout_st;
			}
			else {
				LockoutTimerState = waiting_st;
			}
			break;

		case lockout_st: // start debounce counter
			if (lockoutCounter==LOCKOUT_DURATION) {
				LockoutTimerState = waiting_st;
				lockoutTimer_stop();
			}
			else {
				LockoutTimerState = lockout_st;
			}
			break;

		default:
			LockoutTimerState = waiting_st;
			break;
	}
}
