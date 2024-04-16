package com.isen.esp_joystick;

import android.Manifest;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.Set;

public class BluetoothActivity extends AppCompatActivity {

    private static final int REQUEST_ENABLE_BT = 1;

    private static final int REQUEST_BLUETOOTH_PERMISSION = 3;

    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<String> pairedDevices;
    private ArrayAdapter<String> pairedDevicesAdapter;
    private ListView listView;
    private final String btDeviceName = "Teleprop_Magnetique";
    private Dialog customDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        listView = findViewById(R.id.listView);
        pairedDevices = new ArrayList<>();
        pairedDevicesAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, pairedDevices);
        listView.setAdapter(pairedDevicesAdapter);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth not supported", Toast.LENGTH_SHORT).show();
            finish();
        } else {
            if (!bluetoothAdapter.isEnabled()) {
                showCustomPopup();
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            } else {
                checkBluetoothPermission();

            }
        }

        listView.setOnItemClickListener((parent, view, position, id) -> {
            String selectedDevice = pairedDevicesAdapter.getItem(position);
            String[] deviceInfo = selectedDevice.split("\n");
            String macAddress = deviceInfo[1]; // Récupérer l'adresse MAC du périphérique sélectionné
            returnDeviceName(macAddress);

        });

    }
    private void checkBluetoothPermission() {
        if (ContextCompat.checkSelfPermission(this,
                android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // Si la permission n'est pas accordée, demandez-la à l'utilisateur.
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.BLUETOOTH_CONNECT},
                    REQUEST_BLUETOOTH_PERMISSION);
        } else {
            // La permission est déjà accordée, vous pouvez continuer à utiliser le Bluetooth.
            showPairedDevices();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_BLUETOOTH_PERMISSION) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                // Permission accordée, vous pouvez continuer à utiliser le Bluetooth.
                showPairedDevices();
            } else {
                // Permission refusée, gérer cela en conséquence.
                Toast.makeText(this, "Bluetooth permission denied", Toast.LENGTH_SHORT).show();
            }
        }
    }



    private void showPairedDevices() {
        Set<BluetoothDevice> pairedDeviceSet = bluetoothAdapter.getBondedDevices();
        if (pairedDeviceSet.size() > 0) {
            for (BluetoothDevice device : pairedDeviceSet) {
                pairedDevices.add(device.getName() + "\n" + device.getAddress());
                if(device.getName().equals(btDeviceName)){
                    returnDeviceName(device.getAddress());
                }
            }
            pairedDevicesAdapter.notifyDataSetChanged();
        } else {
            Toast.makeText(this, "No paired devices found", Toast.LENGTH_SHORT).show();
        }
    }
    private void returnDeviceName(String macAddress){
        // Retourner à MainActivity avec l'adresse MAC comme extra

        Intent resultIntent = new Intent();
        resultIntent.putExtra("MAC_ADDRESS", macAddress);
        setResult(RESULT_OK, resultIntent);
        finish(); // Terminer cette activité et revenir à MainActivity

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode == RESULT_OK) {
                showPairedDevices();
                dismissPopup();
            } else {
                Toast.makeText(this, "Bluetooth activation canceled", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    private void showCustomPopup() {// type 1 = chute et 2 = inactif

            // Créer la boîte de dialogue personnalisée
            customDialog = new Dialog(BluetoothActivity.this, R.style.FullScreenDialog);
            customDialog.setContentView(R.layout.no_bt_popup);

        // Afficher la boîte de dialogue
        customDialog.show();
    }


    private void dismissPopup() {
        if (customDialog != null && customDialog.isShowing()) {
            customDialog.dismiss();
        }
    }
}
