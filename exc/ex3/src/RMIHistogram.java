import java.rmi.AlreadyBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;

public class RMIHistogram implements Binder, RemoteHistogram {
    Map<Integer, int[]> histograms = new ConcurrentHashMap<>();

    @Override
    public void bind(String serviceName) {
        try {
            RemoteHistogram stub = (RemoteHistogram) UnicastRemoteObject.exportObject(this, 0);
            Registry registry = LocateRegistry.getRegistry(1099);
            registry.bind(serviceName, stub);
        } catch (RemoteException e) {
            e.printStackTrace();
        } catch (AlreadyBoundException e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public int createHistogram(int bins) throws RemoteException {
        Random rand = new Random();
        int nextId = rand.nextInt();
        while (histograms.containsKey(nextId))
            nextId = rand.nextInt();
        histograms.put(nextId, new int[bins]);
        return nextId;
    }

    @Override
    public void addToHistogram(int histogramID, int value) throws RemoteException {
        synchronized (this) {
            histograms.get(histogramID)[value]++;
        }
    }

    @Override
    public int[] getHistogram(int histogramID) throws RemoteException {
        return histograms.get(histogramID);
    }
}
