mrPour.controller('dashboardController', function ($scope, $rootScope, $http, localStorageService, $location, $anchorScroll) {

    $scope.modalShown = false;

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

    $scope.scrollTo = function(id) {
        $location.hash(id);
        $anchorScroll();
    };

});