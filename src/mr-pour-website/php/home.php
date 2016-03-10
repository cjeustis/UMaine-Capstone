<!-- NavBar -->
<nav class="navbar navbar-default navbar-fixed-top">
	<div class="container">
		<div class="navbar-header">
			<a class="navbar-brand" href="#/dashboard">Mr. Pour</a>
		</div>
		<ul class="nav navbar-nav">
			<li><a class="nav-button" ng-click="scrollTo('about')">ABOUT</a></li>
			<li><a class="nav-button" ng-click="scrollTo('services')"">SERVICES</a></li>
			<li><a class="nav-button" ng-click="scrollTo('contact')"">CONTACT</a></li>
		</ul>
		<ul ng-hide="isAuthenticated" class="nav navbar-nav navbar-right">
			<li><a><span class="glyphicon glyphicon-user"></span> <?php echo $row['user_name']; ?></a></li>
			<li><a id="logout-id"><span class="glyphicon glyphicon-log-in"></span> Logout</a></li>
		</ul>

	</div>
</nav>