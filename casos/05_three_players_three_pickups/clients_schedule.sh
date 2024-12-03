echo 'Pepito' >> __client_0_pipe__
echo 'Junasito' >> __client_1_pipe__


echo 'c' >> __client_0_pipe__

echo '2' >> __client_0_pipe__
echo 'match_name' >> __client_0_pipe__
echo '3' >> __client_0_pipe__
# Para asegurar que el orden de los pickups sea tal como esta escrito
sleep 0.5
echo 'j' >> __client_1_pipe__
echo '1' >> __client_1_pipe__

echo 'Exit' >> __client_0_pipe__
echo 'Exit' >> __client_1_pipe__