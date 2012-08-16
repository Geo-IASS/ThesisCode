#!/usr/bin/perl

################################################################################
#
# Script to convert MATLAB profiler output into CSV format. Processes data as
# produced by the `josh_profile_block_size' script.
#
# Usage:
#     ./block_size.pl input
#
# Input:  Base directory where the profiler output is found. Should contain 
#         subdirectories named after the data sets that the data pertains to.
#         Each data set subdirectory should contain a single subdirectory for 
#         each iteration of the Matlab code. The input format should be a 
#         directory hierarchy with the following structure:
#             `{data-set}/{iteration}/{block-size}`
# Output: Profiler data in CSV format. Outputs to STDOUT.
#
################################################################################

use strict;
use warnings;

use File::Spec;

use constant PROFILE_HTML_FILE => "file0.html";
use constant MATLAB_LOG_FILE   => "matlab_output.log";
use constant OUTPUT_HEADER     => "Data set,Block size,log(Block size),Iteration,Dimensions,Vectors,Total time,Projected dimensions,Projected vectors,Function time,Distance calls,Pruned\n";

require "./util.pl";

# The argument should be the base directory for the profiling data
scalar(@ARGV) >= 1 || die("No directory specified!\n");
my $base_dir = $ARGV[0];
-d $base_dir || die("Directory doesn't exist: $base_dir\n");

# Get data sets from subdirectories below base directory
my @dataset_dirs = next_subdirectory_level($base_dir);

# Print header of output
print(OUTPUT_HEADER);

# Loop through each data set
for my $dataset_dir (@dataset_dirs) {
    my $dataset = strip_directory($dataset_dir);
    
    # Get iterations from next subdirectory level
    my @iteration_dirs = next_subdirectory_level($dataset_dir);
    
    # Loop through each iteration subdirectory
    for my $iteration_dir (@iteration_dirs) {
        my $iteration = strip_directory($iteration_dir);
        
        # Get block sizes from next subdirectory level
        my @block_size_dirs = next_subdirectory_level($iteration_dir);
        
        # Loop through each profile subdirectory
        for my $block_size_dir (@block_size_dirs) {
            my $block_size = parse_block_size($block_size_dir);
            my $log_block_size = log10($block_size);
            
            my $dimensions;
            my $vectors;
            my $projected_dimensions;
            my $projected_vectors;
            my $distance_calls;
            my $pruned;
            my $total_time;
            my $function_time;
            
            # Retrieve the input HTML file
            my $profile_html_file = File::Spec->catfile($block_size_dir, PROFILE_HTML_FILE);
            my @data_rows = get_table_rows($profile_html_file);

            # Output the data, looping through each row
            foreach my $row (@data_rows) {
                my $function_name = parse_function_name($row);
                my $time = parse_function_total_time($row);
                
                if ($function_name =~ m/^commute_distance_anomaly_profiling$/) {
                    $total_time = $time;
                } elsif ($function_name =~ m/^TopN_Outlier_Pruning_Block/) {
                    $function_time = $time;
                }
            }
            
            # Parse the MATLAB log file
            my $log_file = $block_size_dir . "/" . MATLAB_LOG_FILE;
            open(LOG, $log_file) || die("Could not open $log_file");
            while (my $line = <LOG>) {                
                if ($line =~ m/Data set dimensions\s*=\s*([0-9]+)\*([0-9]+)/) {
                    $vectors = $1;
                     $dimensions = $2;
                } elsif ($line =~ m/Projected data set dimensions\s*=\s*([0-9]+)\*([0-9]+)/) {
                    $projected_vectors = $1;
                    $projected_dimensions = $2;
                } elsif ($line =~ m/Number of pruned vectors\s*=\s*([0-9]+)/) {
                    $pruned = $1;
                } elsif ($line =~ m/Calls to distance function\s*=\s*([0-9]+)/) {
                    $distance_calls = $1;
                }
            }
            close(LOG);
            
            # Print output
            print("\"$dataset\",$block_size,$log_block_size,$iteration,$dimensions,$vectors,$total_time,$projected_dimensions,$projected_vectors,$function_time,$distance_calls,$pruned\n");
        }
    }
}