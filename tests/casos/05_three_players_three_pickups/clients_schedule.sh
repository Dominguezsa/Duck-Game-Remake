echo 'Pepito' >> __client_0_pipe__
echo 'Junasito' >> __client_1_pipe__
echo 'Pablito' >> __client_2_pipe__


echo 'c' >> __client_0_pipe__
sleep 1
echo '2' >> __client_0_pipe__
sleep 1
echo 'match_name' >> __client_0_pipe__
sleep 1
echo '3' >> __client_0_pipe__
# Para asegurar que el orden de los pickups sea tal como esta escrito
sleep 1
echo 'j' >> __client_1_pipe__
sleep 1
echo '1' >> __client_1_pipe__
sleep 1
echo 'j' >> __client_2_pipe__
sleep 1
echo '1' >> __client_2_pipe__

echo 'Exit' >> __client_0_pipe__
echo 'Exit' >> __client_1_pipe__
echo 'Exit' >> __client_2_pipe__