#!/bin/bash
T1=./tmp1
T2=./tmp2
rm -f occurlflags.c
rm -f $T1 $T2
cpp -I/usr/include/curl </usr/include/curl/curl.h >$T1
sed -e '/CURLOPT_LASTENTRY/d' -e '/CURLOPT_/p;d' <$T1 >$T2
rm $T1
sed -e 's/[ \t][ \t]*//g' -e 's/,/ /g' <$T2 |sort >$T1
rm $T2
sed -e 's/\([^=]*\)[=]\(.*\)/\1 \2/' <$T1 >$T2
rm $T1

while read p; do
COP=`echo $p | cut -d' ' -f1`
CN=`echo $p | cut -d' ' -f2`
CN=`echo $CN | bc`
echo "{\"$COP\",$COP,$CN,CF_UNKNOWN}," >>$T1
done <$T2
sed  -e "/^XXXXXXXX/r$T1" -e '/^XXXXXXXX/d' <occurlflags.c.in >occurlflags.c
#rm -f $T1 $T2
exit
