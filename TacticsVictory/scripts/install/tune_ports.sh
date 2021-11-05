iptables --policy INPUT ACCEPT
iptables --policy FORWARD ACCEPT
iptables --policy OUTPUT ACCEPT

iptables --table filter --flush
iptables --table nat --flush
iptables --table mangle --flush
iptables --table raw --flush
iptables --table security --flush

iptables --table filter --delete-chain
iptables --table nat --delete-chain
iptables --table mangle --delete-chain
iptables --table raw --delete-chain
iptables --table security --delete-chain

iptables --append INPUT --in-interface lo --jump ACCEPT

iptables --append INPUT --match conntrack --ctstate ESTABLISHED,RELATED --jump ACCEPT

iptables --append INPUT --protocol tcp --dport 21 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 22 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 80 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 443 --jump ACCEPT

iptables --append INPUT --protocol tcp --dport 40000 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40000 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 40001 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40001 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 40002 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40002 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 40003 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40003 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 40004 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40004 --jump ACCEPT
iptables --append INPUT --protocol tcp --dport 40005 --jump ACCEPT
iptables --append INPUT --protocol udp --dport 40005 --jump ACCEPT

iptables --policy INPUT DROP
iptables --policy FORWARD DROP
iptables --policy OUTPUT ACCEPT

netfilter-persistent save
