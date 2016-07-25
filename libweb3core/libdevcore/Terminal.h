#pragma once

namespace dev
{
namespace con
{

#if defined(_WIN32)

#define EleReset ""       // Text Reset

#define EleReset ""       // Text Reset

	// Regular Colors
#define EleBlack	""    // Black
#define EleCoal		""    // Black
#define EleGray		""    // White
#define EleWhite	""    // White
#define EleMaroon	""    // Red
#define EleRed		""    // Red
#define EleGreen	""    // Green
#define EleLime		""    // Green
#define EleOrange	""    // Yellow
#define EleYellow	""    // Yellow
#define EleNavy		""    // Blue
#define EleBlue		""    // Blue
#define EleViolet	""    // Purple
#define ElePurple	""    // Purple
#define EleTeal		""    // Cyan
#define EleCyan		""    // Cyan

#define EleBlackBold	""      // Black
#define EleCoalBold		""      // Black
#define EleGrayBold		""      // White
#define EleWhiteBold	""      // White
#define EleMaroonBold	""      // Red
#define EleRedBold		""      // Red
#define EleGreenBold	""      // Green
#define EleLimeBold		""      // Green
#define EleOrangeBold	""      // Yellow
#define EleYellowBold	""      // Yellow
#define EleNavyBold		""      // Blue
#define EleBlueBold		""      // Blue
#define EleVioletBold	""      // Purple
#define ElePurpleBold	""      // Purple
#define EleTealBold		""      // Cyan
#define EleCyanBold		""      // Cyan

	// Background
#define EleOnBlack		""       // Black
#define EleOnCoal		""		 // Black
#define EleOnGray		""       // White
#define EleOnWhite		""		 // White
#define EleOnMaroon		""       // Red
#define EleOnRed		""       // Red
#define EleOnGreen		""       // Green
#define EleOnLime		""		 // Green
#define EleOnOrange		""       // Yellow
#define EleOnYellow		""		 // Yellow
#define EleOnNavy		""       // Blue
#define EleOnBlue		""		 // Blue
#define EleOnViolet		""       // Purple
#define EleOnPurple		""		 // Purple
#define EleOnTeal		""       // Cyan
#define EleOnCyan		""		 // Cyan

	// Underline
#define EleBlackUnder	""       // Black
#define EleGrayUnder	""       // White
#define EleMaroonUnder	""       // Red
#define EleGreenUnder	""       // Green
#define EleOrangeUnder	""       // Yellow
#define EleNavyUnder	""       // Blue
#define EleVioletUnder	""       // Purple
#define EleTealUnder	""       // Cyan

#else

#define EleReset "\x1b[0m"       // Text Reset

// Regular Colors
#define EleBlack "\x1b[30m"        // Black
#define EleCoal "\x1b[90m"       // Black
#define EleGray "\x1b[37m"        // White
#define EleWhite "\x1b[97m"       // White
#define EleMaroon "\x1b[31m"          // Red
#define EleRed "\x1b[91m"         // Red
#define EleGreen "\x1b[32m"        // Green
#define EleLime "\x1b[92m"       // Green
#define EleOrange "\x1b[33m"       // Yellow
#define EleYellow "\x1b[93m"      // Yellow
#define EleNavy "\x1b[34m"         // Blue
#define EleBlue "\x1b[94m"        // Blue
#define EleViolet "\x1b[35m"       // Purple
#define ElePurple "\x1b[95m"      // Purple
#define EleTeal "\x1b[36m"         // Cyan
#define EleCyan "\x1b[96m"        // Cyan

#define EleBlackBold "\x1b[1;30m"       // Black
#define EleCoalBold "\x1b[1;90m"      // Black
#define EleGrayBold "\x1b[1;37m"       // White
#define EleWhiteBold "\x1b[1;97m"      // White
#define EleMaroonBold "\x1b[1;31m"         // Red
#define EleRedBold "\x1b[1;91m"        // Red
#define EleGreenBold "\x1b[1;32m"       // Green
#define EleLimeBold "\x1b[1;92m"      // Green
#define EleOrangeBold "\x1b[1;33m"      // Yellow
#define EleYellowBold "\x1b[1;93m"     // Yellow
#define EleNavyBold "\x1b[1;34m"        // Blue
#define EleBlueBold "\x1b[1;94m"       // Blue
#define EleVioletBold "\x1b[1;35m"      // Purple
#define ElePurpleBold "\x1b[1;95m"     // Purple
#define EleTealBold "\x1b[1;36m"        // Cyan
#define EleCyanBold "\x1b[1;96m"       // Cyan

// Background
#define EleOnBlack "\x1b[40m"       // Black
#define EleOnCoal "\x1b[100m"   // Black
#define EleOnGray "\x1b[47m"       // White
#define EleOnWhite "\x1b[107m"   // White
#define EleOnMaroon "\x1b[41m"         // Red
#define EleOnRed "\x1b[101m"     // Red
#define EleOnGreen "\x1b[42m"       // Green
#define EleOnLime "\x1b[102m"   // Green
#define EleOnOrange "\x1b[43m"      // Yellow
#define EleOnYellow "\x1b[103m"  // Yellow
#define EleOnNavy "\x1b[44m"        // Blue
#define EleOnBlue "\x1b[104m"    // Blue
#define EleOnViolet "\x1b[45m"      // Purple
#define EleOnPurple "\x1b[105m"  // Purple
#define EleOnTeal "\x1b[46m"        // Cyan
#define EleOnCyan "\x1b[106m"    // Cyan

// Underline
#define EleBlackUnder "\x1b[4;30m"       // Black
#define EleGrayUnder "\x1b[4;37m"       // White
#define EleMaroonUnder "\x1b[4;31m"      // Red
#define EleGreenUnder "\x1b[4;32m"       // Green
#define EleOrangeUnder "\x1b[4;33m"      // Yellow
#define EleNavyUnder "\x1b[4;34m"        // Blue
#define EleVioletUnder "\x1b[4;35m"      // Purple
#define EleTealUnder "\x1b[4;36m"        // Cyan

#endif

}

}
