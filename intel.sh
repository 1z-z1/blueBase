#!/bin/bash

# Get project name from current directory
PROJECT_NAME=$(basename "$PWD")

# Find the next available report number
COUNTER=1
while [ -f "intelReport${COUNTER}.txt" ]; do
    ((COUNTER++))
done
OUTPUT="intelReport${COUNTER}.txt"

# Create header with project name and date
echo "Intelligence Report for Project: ${PROJECT_NAME}" > $OUTPUT
echo "Generated: $(date)" >> $OUTPUT
echo "Report Number: ${COUNTER}" >> $OUTPUT
echo "----------------------------------------" >> $OUTPUT

# 3rdparty directory (limited depth)
echo -e "\n[Project: ${PROJECT_NAME}] - 3rdparty Directory Structure (Depth 3)" >> $OUTPUT
echo "Information On More Files Available Upon Request" >> $OUTPUT
tree --noreport -L 2 ./3rdparty 2>/dev/null >> $OUTPUT

# Backup directory (limited depth)
echo -e "\n[Project: ${PROJECT_NAME}] - Backup Directory Structure (Depth 2)" >> $OUTPUT
echo "Backup Files Available Upon Request" >> $OUTPUT
tree --noreport -L 1 ./bak 2>/dev/null >> $OUTPUT

# Main project structure (excluding 3rdparty, bak, and build)
echo -e "\n[Project: ${PROJECT_NAME}] - Main Project Structure (Depth 9)" >> $OUTPUT
echo "Reference Files Available Upon Request" >> $OUTPUT
tree --noreport -L 9 . -I "3rdparty|bak|build" 2>/dev/null >> $OUTPUT

# Source files
echo -e "\n[Project: ${PROJECT_NAME}] - Source Files (.cpp, .h)" >> $OUTPUT
echo "Reference Files Available Upon Request" >> $OUTPUT
find . -type f \( -name "*.cpp" -o -name "*.h" \) -not -path "./3rdparty/*" -not -path "./bak/*" -not -path "./build/*" 2>/dev/null >> $OUTPUT

# Shader files
echo -e "\n[Project: ${PROJECT_NAME}] - Shader Files (.sc, .bin)" >> $OUTPUT
echo "Reference Files Available Upon Request" >> $OUTPUT
find . -type f \( -name "*.sc" -o -name "*.bin" \) -not -path "./3rdparty/*" -not -path "./bak/*" -not -path "./build/*" 2>/dev/null >> $OUTPUT

echo -e "\nIntelligence report generated: $OUTPUT"
