
#include "SciDataParser.h"
#include "CDataSet.h"
#include <numeric>


char * textFileRead(char const * fn)
{	
	FILE * fp;
	char * content = NULL;
	size_t count = 0;

	if (fn != NULL)
	{
		fp = fopen(fn,"rt");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0)
			{
				content = new char[count+1];
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}

	return content;
}


void SciDataParserSimpleTXT::Load()
{
	// read the files into buffers
	char* dataBuf = textFileRead(FileName.c_str());
	if (! dataBuf)
		return;

	float time, O2, x, y, z, temp, d1;
	char *tok;
	int line = 0;
	tok = strtok(dataBuf, "\n");

	DataSet->Points.AddField("o2");
	DataSet->Points.AddField("temp");
	DataSet->Points.AddField("d1");
	DataSet->Points.AddField("x");
	DataSet->Points.AddField("y");
	DataSet->Points.AddField("z");

	while(tok != NULL)
	{
		if(sscanf(tok, "%f %f %f %f %f %f %f", &time, &x, &z, &y, &temp, &O2, &d1) <= 0)
		{
			printf("Error: Malformed file on line %d\n", line);
			return;
		}

		// push scidata into list
		STable::SRow & Row = DataSet->Points.AddRow();
		Row.GetField("o2") = O2;
		Row.GetField("temp") = temp;
		Row.GetField("d1") = d1;
		Row.GetField("x") = x;
		Row.GetField("y") = -y; // Flip y from depth to coordinate
		Row.GetField("z") = z;

		// move onto next line
		tok = strtok(NULL, "\n");
		line++;
	}
}
