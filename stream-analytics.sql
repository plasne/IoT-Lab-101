SELECT
    AVG([Value]) as [Value]
INTO
    [pelasne-iothub-output-all]
FROM
    [pelasne-iothub-input]
GROUP BY
    SlidingWindow(second, 20), [DeviceId]

SELECT
    [DeviceId],
    [Value],
    System.Timestamp as [Timestamp]
INTO
    [pelasne-iothub-sql]
FROM
    [pelasne-iothub-input];
