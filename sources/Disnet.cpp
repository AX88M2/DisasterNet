#include <enet/enet.h>
#include "Disnet.hpp"

#include "windows.h"

#include <cstdio>
#include <format>

ENetHost* host;
ENetPeer* peer;
ENetEvent event;

double disnet_init() {
    if (enet_initialize())
        return 0;

    host = enet_host_create(nullptr, 1, 2, 5000000, 5000000);

    OutputDebugStringA("enet_initialize() succeeded\n");

    if ( !host )
        return 0;

    atexit(disnet_uninit);
    return 1;
}

void disnet_uninit() {
    if (peer)
    {
        enet_peer_disconnect_now(peer, 0);
        OutputDebugStringA("enet_peer_disconnect_now() succeeded\n");
        peer = nullptr;
    }
    if (host)
    {
        enet_host_destroy(host);
        OutputDebugStringA("enet_host_destroy() succeeded\n");
        peer = nullptr;
    }
    enet_deinitialize();
}

double disnet_connect(const char *ip, double port) {
    ENetAddress address;
    address.port = (int)(port);

    if ( enet_address_set_host_ip(&address, ip) < 0 && enet_address_set_host(&address, ip) < 0 )
        return 0.0;

    peer = enet_host_connect(host, &address, 2u, 0);

    return (double)(peer != nullptr);
}

double disnet_poll(char *buffer) {
    if (enet_host_service(host, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            return 1.0;
        }

        if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            memcpy(buffer, &event.data, sizeof(event.data));
            return 2.0;
        }

        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            memcpy(buffer, event.packet->data, event.packet->dataLength);
            enet_packet_destroy(event.packet);
            return ((event.packet->flags & 1) == 0) + 3;
        }
    }
    return 0.0;
}

void disnet_reset() {
    if (peer) {
        enet_peer_disconnect_now(peer, 0);
        peer = nullptr;
    }
}

void disnet_send(char *data, double length, double reliable) {
    ENetPacket *packet = enet_packet_create(data, (unsigned int)length, reliable != 0.0);
    enet_peer_send(peer, reliable == 0.0, packet);
    printf("data: %p | size: %f %i", data, length, reliable != 0.0);
}
