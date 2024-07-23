#!/bin/bash

REPORT_DIR=../Report

for TEST_EXE in $(ls *A *W); do 
	./$TEST_EXE --gtest_output=xml:${REPORT_DIR}/${TEST_EXE}.linux.xml
done
