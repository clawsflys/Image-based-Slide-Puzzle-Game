README

Overview

KHMTHA045

This program implements a tile-based sliding puzzle that processes an input PGM image, divides it into tiles, simulates a sequence of valid moves, and outputs individual board states along with a summary image. The program adheres to specified core requirements (85%) and mastery requirements (15%).

Files

1. puzzleGame.cpp (Main Program)

Purpose:

	This is the main program file that implements the logic for parsing command-line arguments, managing the TileManager object, and coordinating the program flow.

Key Responsibilities:

	Parses input arguments such as grid size, number of moves, base name for output images, and the summary image name.

	Reads the input PGM image using the PGMimage class.

	Initializes the TileManager object with the input image and grid size.

	Iteratively generates tile board states by invoking TileManager::makeMove().

	Saves each board state as a PGM image using TileManager::saveBoardState().

	Generates a summary image consolidating all board states using TileManager::createSummaryImage() (if the -x flag is provided).

2. PGMimage.h (Header File)

Purpose:

	Defines the interface for the PGMimage class used to handle PGM images.

Key Responsibilities:

	Contains declarations for methods to:

		Read PGM images from disk.

		Write PGM images to disk.

		Access image dimensions and pixel buffer.

3. PGMimage.cpp (PGM Image Implementation)
Purpose:

	Implements the methods declared in PGMimage.h.

Key Responsibilities:

	Reads PGM images, validates the file format, and loads pixel data into a buffer.

	Writes pixel data from the buffer into a PGM file.

	Provides methods for accessing image dimensions and pixel data.

4. TileManager (Integrated into puzzleGame.cpp)
Purpose:

	Encapsulates all information about the sliding puzzle grid and manages operations on the tile board.

Key Responsibilities:

	Initializes the tile board by subdividing the input PGM image into tiles.

	Ensures the bottom-right corner is initially empty.

	Computes valid tile moves and tracks the empty tile's position.

	Manages memory for each tile, including cleanup in the destructor.

	Generates PGM images for each board state after a move.

	Creates a summary image consolidating all board states (mastery requirement).

5. Tile (Integrated into puzzleGame.cpp)
Purpose:

	Represents individual tiles in the sliding puzzle.

Key Responsibilities:

	Stores pixel data for a single tile.

	Provides methods to set and retrieve pixel data.

	Manages memory for the dynamically allocated pixel array.

How the Files Fit Together
puzzleGame.cpp:

	Acts as the entry point for the program.

	Uses PGMimage to process the input image and initialize the tile grid.

	Invokes methods in the TileManager class to generate moves, save board states, and create the summary image.

	Parses and validates command-line arguments to control program behavior.

PGMimage.h and PGMimage.cpp:

	Handle the low-level details of reading and writing PGM images.

	Provide an abstraction for working with image data, which the TileManager and Tile classes rely on.

TileManager:

	Manages the overall state of the sliding puzzle, including tile positions and moves.

	Relies on the Tile class to handle pixel data for individual tiles.

	Uses the PGMimage class to generate output images for each move and the summary.

Tile:

	Encapsulates image data for a single tile, making it easier for TileManager to manage the puzzle grid.

How to Compile and Run

Compilation

	Use the following command to compile the program:

	bash
		g++ -o puzzleGame puzzleGame.cpp PGMimage.cpp
		Running the Program
		To run the program, use:

	bash
		./puzzleGame -s <grid size> -o <output base name> -n <number of moves> [-x <summary name>] <input image>

Example:
	bash
	./puzzleGame -s 5 -o output -n 10 -x summary inputImage.pgm -s 5: Specifies a 5x5 grid.

	-o output: Sets the base name for output images (e.g., output1.pgm, output2.pgm).

	-n 10: Simulates 10 valid moves.

	-x summary: Generates a summary image named summary.pgm consolidating all board states.

	inputImage.pgm: Path to the input PGM image.

Notes for Troubleshooting
	Ensure the input image is a valid PGM file and large enough to fit the specified grid size.

	If no summary image is generated, verify the -x flag is used correctly and sufficient disk space is available.

	The program does not support smart pointers, per the assignment requirements, so make sure all new allocations are matched with delete.
