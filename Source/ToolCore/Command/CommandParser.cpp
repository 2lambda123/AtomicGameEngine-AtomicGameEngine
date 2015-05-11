
#include "CommandParser.h"

#include "NewProjectCmd.h"
#include "PlatformAddCmd.h"
#include "BuildCmd.h"
#include "ImportCmd.h"

namespace ToolCore
{

CommandParser::CommandParser(Context* context) : Object(context)
{

}

CommandParser::~CommandParser()
{

}

Command* CommandParser::Parse(const Vector<String>& arguments)
{
    Command* cmd = NULL;

    for (unsigned i = 0; i < arguments.Size(); ++i)
    {
        if (arguments[i].Length())
        {
            String argument = arguments[i].ToLower();

            if (argument == "new")
            {
                cmd = new NewProjectCmd(context_);
            }
            else if (argument == "build")
            {
                cmd = new BuildCmd(context_);
            }
            else if (argument == "platform-add")
            {
                cmd = new PlatformAddCmd(context_);
            }
            else if (argument == "import")
            {
                cmd = new ImportCmd(context_);
            }

        }

        if (cmd)
        {
            if (cmd->Parse(arguments, i, errorMsg_))
                return cmd;

            cmd->ReleaseRef();
            break;
        }

    }

    return NULL;
}

}

