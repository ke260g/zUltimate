[TOC]
udp 53
## servers in dns query pipeline
recursive resolver (recursor) >
root nameserver >
top level domain server (TLD) >
authoritative nameserver

### root nameserver
global 13 types
return TLD nameserver table
    .com <=> server-ip
    .org <=> server-ip
manage by Internet Corporation for Assigned Names and Numbers (ICANN)

### TLD nameserver
index by the last dot in a url
each TLD nameserver parse only one of .com, .org, or .net
return authoritative nameserver of
  url1.com <=> server-ip
  url2.com <=> server-ip
  url3.com <=> server-ip
manage by Internet Assigned Numbers Authority (IANA)
  Generic top-level domains: .com, .org, .net, .edu, and .gov.
  Country code top-level domains: uk, .us, .ru, .jp, .du

### authoritative nameserver
return ip

## The 8 steps in a DNS lookup (iterative method)
1. A user types ‘example.com’ into a web browser and the query travels into the Internet and is received by a DNS recursive resolver.
2. The resolver then queries a DNS root nameserver (.).
3. The root server then responds to the resolver with the address of a Top Level Domain (TLD) DNS server (such as .com or .net), which stores the 4. information for its domains. When searching for example.com, our request is pointed toward the .com TLD.
4. The resolver then makes a request to the .com TLD.
5. The TLD server then responds with the IP address of the domain’s nameserver, example.com.
6. Lastly, the recursive resolver sends a query to the domain’s nameserver.
7. The IP address for example.com is then returned to the resolver from the nameserver.
8. The DNS resolver then responds to the web browser with the IP address of the domain requested initially.

### DNS resolvers lookup procedure pattern
1. non-recursive
2. recursive
```
  +---+ ==[1]=> *~~~* ==[2]=> ... ==[3]=> +---+
  | A |         # B #         ...         | Z |
  +---+ <=[6]== *~~~* <=[5]== ... <=[4]== +---+
```
3. iterative
```
  A --> B  A <-- B
  A --> C  A <-- C
  A --> D  A <-- D
  A --> ?  A <-- ?
  A --> Z  A <-- Z
```
4. combination of above.

## Circular dependencies and glue records
For example, if the authoritative name server for example.org is ns1.example.org, a computer trying to resolve www.example.org first resolves ns1.example.org.

## Record caching

## Reverse lookup
query of the DNS for domain names when the IP address is known
pointer (PTR) request
top-level domain arpa
root servers American Registry for Internet Numbers (ARIN) 

## Broken resolvers
Some large ISPs have configured their DNS servers to violate rules
  disobeying TTLs
  indicating that a domain name does not exist

Virtual Hosting: Multiple hostnames may correspond to a single IP
Load Balanced: Single Hostnames correspond to multiple IP

## DNS message format
two types queries and replies, both have same format
a header and four sections: 
  question, answer, authority, and an additional space
### a header
Identification:
  match responses with queries
Flags:
  a single bit which indicates a query (0) or a reply (1)
  four bits indicating the type of query, or the type of query this message is a response to
  a single bit if the DNS server is authoritative
  a single bit if the client wants to send a recursive query
  a single bit if the replying DNS server supports recursion
  a single bit if the message was truncated for some reason
  four bits for error codes
Number of questions
Number of answers, 
Number of authority resource records (RRs), 
and Number of additional RRs
### section question
the domain name and type of record (A, AAAA, MX, TXT, etc.) being resolved
### section answer
resource records