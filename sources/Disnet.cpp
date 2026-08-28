#include "enet.h"
#include "yysdk/YYGML.h"
#include "Disnet.hpp"

#include "windows.h"

#include <cstdio>
#include <format>
#include <iostream>
#include <ostream>

ENetHost* client;
ENetPeer* peer;
ENetEvent event;

double disnet_init() {
    if (enet_initialize())
        return 0;

    client = enet_host_create(nullptr, 1, 2, 5000000, 5000000);

    OutputDebugStringA("enet_initialize() succeeded\n");

    if ( !client )
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
    if (client)
    {
        enet_host_destroy(client);
        OutputDebugStringA("enet_host_destroy() succeeded\n");
        peer = nullptr;
    }
    enet_deinitialize();
}

double disnet_connect(const char *ip, double port) {
    ENetAddress address;
    address.port = static_cast<uint16_t>(port);

    if ( enet_address_set_host_ip(&address, ip) < 0 && enet_address_set_host(&address, ip) < 0 )
        return 0.0;

    peer = enet_host_connect(client, &address, 2u, 0);

    return peer != nullptr;
}

double disnet_poll(YYRValue *data) {
    if (enet_host_service(client, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            return 1.0;
        }

        if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            //*data = event.data;
            return 2.0;
        }

        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            memcpy(data->ptr, event.packet->data, event.packet->dataLength);
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

void disnet_send(const void *data, double size, double reliable) {
    size_t packet_size = 0;
    if ( size >= 9.223372036854776e18 )
    {
        size = size - 9.223372036854776e18;
        if ( size < 9.223372036854776e18 )
            packet_size = 0x8000000000000000uLL;
    }
    ENetPacket *packet = enet_packet_create(data, packet_size + size, reliable != 0.0);
    enet_peer_send(peer, reliable == 0.0, packet);
    printf("data: %p | size: %f %i", data, size, reliable != 0.0);
}
