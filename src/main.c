#include "main.h"

bool checkAction(const char *s, Action_t *action)
{
    if (strcmp(s, "send") == 0)
    {
        *action = SEND;
        return true;
    }
    if (strcmp(s, "recv") == 0)
    {
        *action = RECEIVE;
        return true;
    }
    return false;
}
bool checkN(const char *s, size_t *n)
{
    char *end;
    *n = strtol(s, &end, 10);

    return *s != '\0' && *end == '\0';
}
bool checkFile(const char *s, FILE **out)
{
    *out = fopen(s, "w");
    if (!*out)
    {
        E_Print("fopen: %s\n", strerror(errno));
        return false;
    }
    return true;
}
bool checkT(const char *s, int32_t *t)
{
    char *end;
    *t = (int32_t) strtol(s, &end, 10);
    if (*t < 0)
        return false;

    return *s != '\0' && *end == '\0';
}

int parseArg(const int argc, char *argv[], Context_t *context)
{
    if (argc < 2 || !checkAction(argv[1], &context->acion))
        return EXIT_FAILURE;

    context->n = 50;
    context->out = stdout;
    context->interactive = false;
    context->t = 2000;

    int opt = getopt(argc, argv, "n:o:it:");
    while (opt != -1)
    {
        switch (opt)
        {
            case 'n':
                if (!checkN(optarg, &context->n))
                    return EXIT_FAILURE;
                break;
            case 'o':
                if (!checkFile(optarg, &context->out))
                    return EXIT_FAILURE;
                break;
            case 'i':
                context->interactive = true;
                break;
            case 't':
                if (!checkT(optarg, &context->t))
                    return EXIT_FAILURE;
                break;
            default:
                E_Print("-%c is not a valid argument!\n", opt);
                return EXIT_FAILURE;
        }
        opt = getopt(argc, argv, "n:o:it:");
    }
    return EXIT_SUCCESS;
}


int main(const int argc, char *argv[])
{
#ifdef Debug
    D_Print("Launched in Debug Mode!\n");
#endif

    Context_t context;
    if (parseArg(argc, argv, &context))
    {
        E_Print("Usage: ProSafe {send|recv} [-n <number_of_packets>] [-o <file_out>] [-i] [-t <millisecond_to_wait>]\n");
        return EXIT_FAILURE;
    }

    fprintf(context.out, BOLD ITALIC GREEN "------Parameters------\n" RESET BLUE "Action:" RESET "      %s\n" BLUE "N packets:" RESET "   %ld\n" BLUE "Output file:" RESET " %s\n" BLUE "Interactive:" RESET " %s\n" BLUE "Wait time: " RESET "  %d" GREEN "\n----------------------\n" RESET, context.acion == SEND ? "Send" : "Receive", context.n, context.out == stdout ? "stdout" : "file", context.interactive ? "yes" : "no", context.t);

    return EXIT_SUCCESS;
}
