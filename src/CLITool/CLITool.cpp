/*
 * Copyright (c) 2019, Austin Goudge.
 * Parts Copyright (c) 2004, Laminar Research
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "../XPTools/version.h"
#include "../Obj/XObjReadWrite.h"
#include <stdio.h>
#include "../Utils/AssertUtils.h"
#include "../Utils/ObjUtils.h"
#include "../Obj/XObjDefs.h"

#if IBM
#include <stdlib.h>
#endif

FILE * err_fi = stdout;

void AssertShellBail(const char * condition, const char * file, int line)
{
	fprintf(err_fi,"ERROR: %s\n", condition);
	fprintf(err_fi,"(%s, %d.)\n", file, line);
	exit(1);
}

int main(int argc, char * argv[])
{
	InstallDebugAssertHandler(AssertShellBail);
	InstallAssertHandler(AssertShellBail);

	if (argc < 2 || !strcmp(argv[1],"-h")) goto help;

	for (int n = 1; n < argc; ++n)
	{
		if (!strcmp(argv[n], "-measure") ||
			!strcmp(argv[n], "--measure"))
		{
            ++n;
            if (n >= argc) goto help;
            const char * f1 = argv[n];
            
            XObj8    obj8;
            float    mins[3];
            float    maxs[3];
            
			if (!XObj8Read(f1, obj8))
            {
                fprintf(err_fi, "Error: unable to open file %s\n", f1);
                exit(1);
            }
			else
            {
                GetObjDimensions8(obj8, mins, maxs);
                printf("%f,%f,%f\n", maxs[0] - mins[0], maxs[1] - mins[1], maxs[2] - mins[2]);
            }
        } else if (!strcmp(argv[n], "--version"))
		{
			print_product_version("CLITool", CLITOOL_VER, CLITOOL_EXTRAVER);
		}
	}

	return 0;
help:
    fprintf(err_fi, "Usage: %s --measure [file]    - Prints dimensions in m: x, y, z\n",argv[0]);
	fprintf(err_fi, "       %s --version           - Prints version information\n",argv[0]);
	return 1;
}
