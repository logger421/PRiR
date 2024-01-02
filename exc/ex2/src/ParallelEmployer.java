import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ParallelEmployer implements Employer {

    private OrderInterface order;
    private final ConcurrentHashMap<Integer, Location> orders = new ConcurrentHashMap<>();
    private final ConcurrentLinkedQueue<Result> orderResults = new ConcurrentLinkedQueue<>();
    private final Set<Location> visitedLocations = new HashSet<>();

    @Override
    public void setOrderInterface(OrderInterface order) {
        this.order = order;
    }

    @Override
    public Location findExit(Location startLocation, List<Direction> allowedDirections) {
        order.setResultListener(new ResultListenerImpl());
        visitedLocations.add(startLocation);

        for (Direction allowedDirection : allowedDirections) {
            Location next = allowedDirection.step(startLocation);
            if (!visitedLocations.contains(next)) {
                int orderID = order.order(next);
                orders.put(orderID, next);
                visitedLocations.add(next);
            }
        }

        synchronized (this) {
            try {
                while (true) {
                    if (orderResults.isEmpty()) {
                        this.wait();
                    } else {
                        Result currentResult = orderResults.poll();
                        Location currentLocation = orders.get(currentResult.orderID());

                        if (currentResult.type() == LocationType.EXIT)
                            return currentLocation;

                        for (Direction direction : currentResult.allowedDirections()) {
                            Location next = direction.step(currentLocation);
                            if (!visitedLocations.contains(next)) {
                                int orderID = order.order(next);
                                orders.put(orderID, next);
                                visitedLocations.add(next);
                            }
                        }
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public class ResultListenerImpl implements ResultListener {
        @Override
        public void result(Result result) {
            synchronized (ParallelEmployer.this) {
                orderResults.add(result);
                ParallelEmployer.this.notify();
            }
        }
    }
}
