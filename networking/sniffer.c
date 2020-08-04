/*
 * Using raw socket o sniff trafiic from a give ethernet interface.
 * Usage:
 *
 *  sniffer eth0
 *
 * Author Wenwei Weng
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <netinet/in.h>		 
#include <netinet/if_ether.h>   
#include <netinet/ip.h>		
#include <netinet/udp.h>	 
#include <netinet/tcp.h>

static void dump_packet(char* buffer, int len);

int main(int argc, char* argv[])
{
    int sock_fd;
    
    sock_fd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
    if(sock_fd<0) {
	printf("\n error in open socket\n");
	return -1;
    }

    // bind socket to given interface if provided
    if (argc == 2) {
	char *intf_name = argv[1];
	printf("\n Bind to interface: %s \n", intf_name);

	struct ifreq interface;
	memset(&interface, 0, sizeof(interface));
	strncpy(interface.ifr_ifrn.ifrn_name, intf_name, IFNAMSIZ);
	if (setsockopt(sock_fd, SOL_SOCKET, SO_BINDTODEVICE, &interface, sizeof(interface)) < 0) {
	    printf("\n Failed to bind to interface: %s\n", intf_name);
	    close(sock_fd);
	    return -1;
	} 
    }
    
    struct sockaddr saddr;
    int saddr_len = sizeof (saddr);
    unsigned char *buffer = (unsigned char *) malloc(65536);

    while(1) {
	memset(buffer,0,65536);
	//Receive a network packet and copy in to buffer
	int buflen = recvfrom(sock_fd, buffer, 65536,0, &saddr,(socklen_t *)&saddr_len);
	if(buflen<0) {
	    printf("\n error in reading recvfrom function\n");
	    return -1;
	}
	dump_packet(buffer, buflen);
    }
}

static void dump_payload(unsigned char* buffer,int buflen)
{
    int i = 0, offset = 0;
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    int iphdrlen =ip->ihl*4;
    if (ip->protocol == 6 /*tcp*/) {
	offset = sizeof(struct tcphdr);
    } else if (ip->protocol == 17 /*udp*/) {
	offset = sizeof(struct udphdr);
    } else {
	//printf("\n Data after IP header:\n");
    }
	
    unsigned char * data = (buffer + iphdrlen  + sizeof(struct ethhdr) + offset);
    int remaining_data = buflen - (iphdrlen  + sizeof(struct ethhdr) + offset);
    printf("Payload Data\n");
    for( i = 0; i < remaining_data; i++) {
	if(i!=0 && i%16==0)
	    printf("\n");
	printf(" %.2X ",data[i]);
    }
    printf("\n");
}

static void dump_ether_header(struct ethhdr *eth)
{
    printf("\n Ethernet Header\n");
    printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
	   eth->h_source[0],eth->h_source[1],eth->h_source[2],
	   eth->h_source[3],eth->h_source[4],eth->h_source[5]);
    printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",
	   eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],
	   eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
    printf("\t|-Protocol : %d\n",eth->h_proto);

}

static void dump_ip_header(struct iphdr *ip)
{
    struct sockaddr_in source, dest;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = ip->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = ip->daddr;
 
    printf("\t|-Version : %d\n",(unsigned int)ip->version);
    printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
    printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
    printf("\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
    printf("\t|-Identification : %d\n",ntohs(ip->id));
    printf("\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
    printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);
    printf("\t|-Header Checksum : %d\n",ntohs(ip->check));
    printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
    printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));    
}

static void dump_tcp_header(struct tcphdr *tcp)
{
    printf("\nTCP Header\n");
    printf("\t|-Source Port          : %u\n",ntohs(tcp->source));
    printf("\t|-Destination Port     : %u\n",ntohs(tcp->dest));
    printf("\t|-Sequence Number      : %u\n",ntohl(tcp->seq));
    printf("\t|-Acknowledge Number   : %u\n",ntohl(tcp->ack_seq));
    printf("\t|-Header Length        : %d DWORDS or %d BYTES\n" ,(unsigned int)tcp->doff,(unsigned int)tcp->doff*4);
    printf("\t|----------Flags-----------\n");
    printf("\t\t|-Urgent Flag          : %d\n",(unsigned int)tcp->urg);
    printf("\t\t|-Acknowledgement Flag : %d\n",(unsigned int)tcp->ack);
    printf("\t\t|-Push Flag            : %d\n",(unsigned int)tcp->psh);
    printf("\t\t|-Reset Flag           : %d\n",(unsigned int)tcp->rst);
    printf("\t\t|-Synchronise Flag     : %d\n",(unsigned int)tcp->syn);
    printf("\t\t|-Finish Flag          : %d\n",(unsigned int)tcp->fin);
    printf("\t|-Window size          : %d\n",ntohs(tcp->window));
    printf("\t|-Checksum             : %d\n",ntohs(tcp->check));
    printf("\t|-Urgent Pointer       : %d\n",tcp->urg_ptr);
}

static void dump_udp_header(struct udphdr *udp)
{
    printf("\nUDP Header\n");
    printf("\t|-Source Port    	: %d\n" , ntohs(udp->source));
    printf("\t|-Destination Port	: %d\n" , ntohs(udp->dest));
    printf("\t|-UDP Length      	: %d\n" , ntohs(udp->len));
    printf("\t|-UDP Checksum   	: %d\n" , ntohs(udp->check));

}


void dump_packet(char* buffer, int len)
{   
    // extract ethernet header
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    dump_ether_header(eth);
    
    // extract IP header
    unsigned short iphdrlen;
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    dump_ip_header(ip);

    if (ip->protocol == 6 /* TCP*/) {
	struct tcphdr *tcp = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	dump_tcp_header(tcp);
    } else if (ip->protocol == 17 /* UDP*/) {
	struct udphdr *udp = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
	dump_udp_header(udp);
    } else {
	printf("Non TCP/UDP packet received: protocol=%d\n", ip->protocol);
    }
    dump_payload(buffer, len);
}
