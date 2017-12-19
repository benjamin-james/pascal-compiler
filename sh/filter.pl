#!/usr/bin/env perl
use warnings;
use strict;

while (<>) {
	if (/^#0\s+(\S+)/) {
		print "$1\n";
	} elsif (/^#\d+\s+\S+\s+in\s+(\S+)/) {
		print "\t$1\n";
	}
}
