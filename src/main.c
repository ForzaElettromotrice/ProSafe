#include "main.h"

#include <errno.h>
#include <logger.h>
#include <math.h>
#include <netManager.h>

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
        logE(stderr, "fopen: %s\n", strerror(errno));
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
    if (argc < 2 || !checkAction(argv[1], &context->action))
        return EXIT_FAILURE;

    context->n = 50;
    context->out = stdout;
    context->interactive = false;
    context->t = context->action == SEND ? 100 : 6000;
    context->total = 0;

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
                logE(stderr, "-%c is not a valid argument!\n", opt);
                return EXIT_FAILURE;
        }
        opt = getopt(argc, argv, "n:o:it:");
    }
    return EXIT_SUCCESS;
}
void freeContext(const Context_t *context)
{
    if (context->out != stdout)
        fclose(context->out);
}

void callback(PacketType_t packetType, size_t size, u_char *data, void *userData)
{
    Context_t *context = userData;
    uint8_t OUI[3];
    memcpy(OUI, data, 3);
    data += 4;

    if (packetType != Beacon)
    {
        logE(context->out, "PT = %d, s = %d\nReceived something that is not what we excpected!\n", packetType, size);
        return;
    }
    size_t pn;
    memcpy(&pn, data, sizeof(size_t));
    logD(context->out, "Received packet number %ld of %ld\n", pn, context->n);
    context->total++;
}


int sendTest(const Context_t *context)
{
    if (loopPcap())
        return EXIT_FAILURE;

    for (size_t i = 0; i < context->n; ++i)
    {
        logD(context->out, "Sending packet number %ld of %ld...", i + 1, context->n);
        addPacket(Beacon, &i, sizeof(size_t));
        while (!isQueueEmpty())
        {
        }
        logD(context->out, "OK!\n");
        mySleep(context->t * 1000);
    }
    stopPcap();
    return EXIT_SUCCESS;
}
int recvTest(const Context_t *context)
{
    setCallback(callback, (void *) context);

    if (loopPcap())
        return EXIT_FAILURE;

    if (context->interactive)
    {
        logD(context->out, "Waiting for user to close...\n");
        getchar();
        logD(context->out, "Closing...\n");
        stopPcap();
        logD(context->out, "OK!\n");
        logD(context->out, "Total packet received: %ld\n", context->total);
        return EXIT_SUCCESS;
    }

    logD(context->out, "Going to sleep for %d microseconds\n", context->t);
    mySleep(context->t);
    logD(context->out, "Waked up! Closing pcap...");
    stopPcap();
    logD(context->out, "OK!\n");
    logD(context->out, "Total packet received: %ld of %ld\n", context->total, context->n);
    return EXIT_SUCCESS;
}

int main(const int argc, char *argv[])
{
#ifdef Debug
    logD(stdout, "Launched in Debug Mode!\n");
#endif

    Context_t context;
    if (parseArg(argc, argv, &context))
    {
        logE(stderr, "Usage: ProSafe {send|recv} [-n <number_of_packets>] [-o <file_out>] [-i] [-t <millisecond_to_wait>]\n");
        return EXIT_FAILURE;
    }

    logD(context.out, BOLD ITALIC GREEN "\n------Parameters------\n" RESET BLUE "Action:" RESET "      %s\n" BLUE "N packets:" RESET "   %ld\n" BLUE "Output file:" RESET " %s\n" BLUE "Interactive:" RESET " %s\n" BLUE "Wait time: " RESET "  %d" GREEN "\n----------------------\n" RESET, context.action == SEND ? "Send" : "Receive", context.n, context.out == stdout ? "stdout" : "file", context.interactive ? "yes" : "no", context.t);


    if (initPcap() || createHandle("wlan1") || activateHandle())
    {
        cleanPcap();
        return EXIT_FAILURE;
    }

    int out = 0;

    switch (context.action)
    {
        case SEND:
            out = sendTest(&context);
            break;
        case RECEIVE:
            out = recvTest(&context);
            break;
    }

    freeContext(&context);
    return out;
}
