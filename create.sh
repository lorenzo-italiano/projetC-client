p=$PWD

for i in {0..20}
do
  osascript -e 'tell app "Terminal"
      do script "
cd '$p'
./client.sh
Jean'$i'
"
  end tell'
done

