package com.sensor.weatherapi;

import lombok.RequiredArgsConstructor;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

// Mockito (The fakes)
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.time.Instant;
import java.util.List;

import static org.mockito.Mockito.*;

@ExtendWith(MockitoExtension.class)
public class TelemetryUnitTests {
    @Mock
    private SensorRepository repo;
    @InjectMocks
    private SensorService service;

    @Test
    //save data is supposed to build a sensor object, save it to repo then return response. this implies
    //we do when repo.find, it gets a sensor back. after a mocks
    public void saveDataHappyPath() throws Exception {
        Sensor mockS = new Sensor(1L, 24.1, 0.5, 1051.2, Instant.now());
        when(repo.save(any(Sensor.class))).thenAnswer(invocation -> {
            Sensor s = invocation.getArgument(0);
            return new Sensor(1L, 24.1, 0.5, 1051.2, Instant.now());
        });

        SensorResponse mockp = new SensorResponse(null, mockS.getTemp(), mockS.getHumidity(), mockS.getPressure(), null);
        SensorResponse resp = service.saveData(mockp);

        assertEquals(resp.temperature(), mockp.temperature());
        assertEquals(resp.humidity(), mockp.humidity());
        assertEquals(resp.pressure(), mockp.pressure());
        //its supposed to have a timestamp so itll have to not be null so we can use them for the charts
        assertNotNull(resp.id());
        assertNotNull(resp.timestamp());

        verify(repo).save(any(Sensor.class));

    }

    @Test
    public void getAll24HappyPath() throws Exception {
        Sensor mockS = new Sensor(1L, 24.1, 0.5, 1051.2, Instant.now());
         //stubbing the save
        when(repo.findAll()).thenReturn(List.of(mockS));

        List<SensorResponse> resp = service.getAll24();

        assertNotNull(resp);
        assertEquals(1, resp.size());
        assertEquals(mockS.getId(), resp.getFirst().id());
        assertEquals(mockS.getTemp(), resp.getFirst().temperature());
        assertEquals(mockS.getHumidity(), resp.getFirst().humidity());
        assertEquals(mockS.getPressure(), resp.getFirst().pressure());
        assertEquals(mockS.getTimestamp(), resp.getFirst().timestamp());

        verify(repo).findAll();

    }
}

