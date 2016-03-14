var routeLoadingIndicator = function($rootScope){
    return {
        restrict:'E',
        template:"<h1 ng-if='isRouteLoading'>Loading...</h1>",
        link:function(scope, elem, attrs){
            scope.isRouteLoading = false;

            $rootScope.$on('$routeChangeStart', function(){
                scope.isRouteLoading = true;
            });

            $rootScope.$on('$routeChangeSuccess', function(){
                scope.isRouteLoading = false;
            });

            scope.$on("loading-started", function(e) {
                scope.isRouteLoading = true;
            });

            scope.$on("loading-complete", function(e) {
                scope.isRouteLoading = false;
            });
        }
    };
};