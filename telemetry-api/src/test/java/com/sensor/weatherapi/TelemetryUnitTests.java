package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import static org.junit.jupiter.api.Assertions.*;

// Mockito (The fakes)
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.Optional;

import static org.mockito.Mockito.*;

@RequiredArgsConstructor
public class TelemetryUnitTests {
    @Mock
    private final SensorRepository repo;
    @InjectMocks
    private final SensorService service;

    @Test
    //save data is supposed to build a sensor object, save it to repo then return response. this implies
    //we do when repo.find, it gets a sensor back. after a mocks
    public void saveDataHappyPath() throws Exception {
        Sensor mockS = Sensor.builder().temp(24.1).humidity(0.50).pressure(1051.2).build();
        when(repo.save(any(Sensor.class))).thenReturn(mockS); //nbecaise of we save a sensor it sould return mockS

        SensorResponse mockp = new SensorResponse(null, mockS.getTemp(), mockS.getHumidity(), mockS.getTemp(), null);
        SensorResponse resp = service.saveData(mockp);

        assertEquals(resp.temperature(), mockp.temperature());
        assertEquals(resp.humidity(), mockp.humidity());
        assertEquals(resp.pressure(), mockp.pressure());
        //its supposed to have a timestamp so itll have to not be null so we can use them for the charts
        assertNotNull(resp.id());
        assertNotNull(resp.timestamp());

        verify(repo, times(1)).save(any(Sensor.class));

    }

    @Test
    public void getAll24HappyPath() throws Exception {

    }
}

