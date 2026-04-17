// '.map' function labeler made by 'Xx jAmes t xX'
#include <idc.idc>

static ImportMAP() {

	auto fp, fLine, x, i, findStart;
	auto rebaseOffset;

	// get the currently loaded binary file full file path
	auto BinaryFilePath = get_input_file_path();

	// copy the directory and file without the '.xex' extension
	auto MAPFilePath = substr( BinaryFilePath, 0, strlen( BinaryFilePath ) - 4 );

	// add the '.map' extension
	MAPFilePath = MAPFilePath + ".map";


	// open the map file
    fp = fopen( MAPFilePath , "r");

	if( fp == 0 )
	{
		Message( "Couldn't find file '%s'!\r\n", MAPFilePath );
		return;
	}
	else
	{
		Message( "Found file '%s'!\r\n", MAPFilePath );
	}

	// parse the "Preferred load address" from the header and compute
	// the rebase offset to 0x82000000 (standard XEX base)
	rebaseOffset = 0;
	do {
		fLine = readstr(fp);
		if (fLine == -1) break;
		if (strstr(fLine, "Preferred load address") != -1)
		{
			// extract the hex address at the end of the line
			auto prefAddr;
			auto j, addrStr;
			addrStr = "";
			j = strstr(fLine, " is ");
			if (j != -1)
			{
				j = j + 4;
				while (j < strlen(fLine) && fLine[j] != "\r" && fLine[j] != "\n")
				{
					addrStr = addrStr + fLine[j];
					j = j + 1;
				}
				prefAddr = xtol("0x" + addrStr);
				rebaseOffset = 0x82000000 - prefAddr;
				Message("Preferred load address: 0x%X, rebase offset: 0x%X\r\n", prefAddr, rebaseOffset);
			}
			break;
		}
	} while (1);

	// rewind to the beginning for the main parsing loop
	fseek(fp, 0, 0);

	// set this variable so we can skip over the first chunk of data
	findStart = 0;

	// read 1 line at a time and parse the data for the address + name
	do {

		// fLine is a line from the map file
        fLine = readstr(fp);

		// make sure there is data
         if (fLine != -1)
		 {

			 auto varName, varAddress;
			 auto iCount = 0, tempStr = "";

			 // if we haven't gotten the start of the right data yet
			 if(findStart == 0)
			 {
				 if(strstr(fLine, "Lib:Object"))
				 {
					 findStart = 1;

					 // skip the next line
					 fLine = readstr(fp);
				 }
			 }

			 // 0x15 bytes into the line is where the name starts
			 for( i = 0x15; i < strlen(fLine); i = i + 1 )
			 {
				 if(fLine[i] == " ")
				 {
					 iCount = iCount + 1;

					 // if iCount is 1 then we are getting the string name
					 if(iCount == 1)
					 {
						 varName = tempStr;
						 x = i;
						 while(fLine[x] == " " && x < strlen(fLine))
						 {
							 x = x + 1;
						 }
						 i = x - 1;
						 tempStr = "";
					 }
					 // if iCount is 2 then we are getting the address
					 else if(iCount == 2)
						 varAddress = xtol("0x" + tempStr) + rebaseOffset;


					i = i + 1;
				 }

				 // update the string char
				 tempStr = tempStr + fLine[i];

			 }

			set_name(varAddress, varName);
			//Message("'%s' = 0x%X\r\n", varName, varAddress);

		 }
    } while (fLine != -1);

}
		 
static main()
{	
	Message("Attempting to load from the .Map!\r\n");
	ImportMAP();
	
	Message("Done!\r\n");
}
