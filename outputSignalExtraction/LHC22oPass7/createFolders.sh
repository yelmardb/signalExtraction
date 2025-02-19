#!/bin/bash

# Define the cutModes list
cutModes=("cutMode101" "cutMode102" "cutMode103" "cutMode104" "cutMode105" \
          "cutMode106" "cutMode107" "cutMode108" "cutMode109" "cutMode110" \
          "cutMode111" "cutMode112" "cutMode113" "cutMode114" "cutMode115" \
          "cutMode116" "cutMode117")

# Loop through the list and create directories
for mode in "${cutModes[@]}"; do
  if mkdir -p "$mode"; then
    echo "Folder created: $mode"
  else
    echo "Failed to create folder: $mode"
  fi
done

