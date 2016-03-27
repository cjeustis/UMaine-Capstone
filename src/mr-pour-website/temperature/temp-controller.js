mrPour.controller('tempController', function ($scope, $rootScope, $http, localStorageService, $location) {

    $scope.modalShown = false;

    $scope.val = {};
    $scope.val['control'] = 't';

    $scope.logout = function() {

        $.ajax({
            type : 'POST',
            url  : 'php/logout.php',
            beforeSend: function() {
                $scope.modalShown = true;
            },
            success :  function(response) {
                setInterval(function() {
                    $scope.modalShown = false;
                    $scope.$apply();
                    doLogout();
                }, 1000);
            }
        });
    };

    function doLogout () {
        localStorageService.set('auth', false);
        localStorageService.set('expires', (Date.now() + 120000));
        $location.path('#/');
        $scope.$apply();
    }

    $scope.currentlySetTemp = localStorageService.get('currentlySetTemp');
    if ($scope.currentlySetTemp < 35 && $scope.currentlySetTemp > 55) {
        $scope.currentlySetTemp = 35;
        localStorageService.set('currentlySetTemp', $scope.currentlySetTemp);
        $scope.sendUpdatedTemp();
    }

    $('#currentlySetTemp').text($scope.currentlySetTemp);
    $('#currentTempSpan').text($rootScope.temp['temp']);

    // Randomly add a data point every 500ms
    var random = new TimeSeries();
    setInterval(function() {
        random.append(new Date().getTime(), $rootScope.temp['temp']);
        $('#currentTempSpan').text($rootScope.temp['temp']);
    }, 500);

    $scope.decreaseTemp = function() {
        if ($scope.currentlySetTemp > 35) {
            while ($rootScope.isBusy) {

            }
            $scope.modalShown = true;
            $rootScope.isBusy = true;
            $scope.currentlySetTemp--;
            $scope.sendUpdatedTemp();
        }
    };

    $scope.increaseTemp = function() {
        if ($scope.currentlySetTemp < 55) {
            while ($rootScope.isBusy) {

            }
            $scope.modalShown = true;
            $rootScope.isBusy = true;
            $scope.currentlySetTemp++;
            $scope.sendUpdatedTemp();
        }
    };

    $scope.sendUpdatedTemp = function() {
        $('#currentlySetTemp').text($scope.currentlySetTemp);
        localStorageService.set('currentlySetTemp', $scope.currentlySetTemp);
        $scope.val['status'] = $scope.currentlySetTemp;
        // Send updated temp to the avr
        $.ajax({
            type: 'post',
            url: 'php/updateCoolingStatus.php',
            data: $scope.val,
            success: function ( data ) {
                setInterval(function() {
                    $rootScope.isBusy = false;
                    $scope.modalShown = false;
                    $scope.$apply();
                }, 1500);
            }
        });
    };

    var chart = new SmoothieChart({
            horizontalLines: [
                {
                    color:'#B20000',
                    lineWidth:2,
                    value:55
                },
                {
                    color:'#59C3D8',
                    lineWidth:2,
                    value:35
                }
            ],
            minValue: 20,
            maxValue: 70,
            millisPerPixel: 100,
            grid: {
                fillStyle: '#eeeeee',
                millisPerLine: 10000,
                verticalSections: 12
            },
            labels: {
                fillStyle: '#000000',
                fontSize: 15
            },
            timestampFormatter:SmoothieChart.timeFormatter
        }),
        canvas = document.getElementById('smoothie-chart'),
        series = new TimeSeries();

    chart.addTimeSeries(random, {
        lineWidth: 2.0,
        strokeStyle: '#000000'
    });

    chart.streamTo(canvas, 500);
});